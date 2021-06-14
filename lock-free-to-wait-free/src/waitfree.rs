use std::sync::atomic::AtomicPtr;
use std::sync::atomic::Ordering;
use std::sync::Arc;
use std::sync::Mutex;

use crate::atomic::VersionedCas;
use crate::cas::CasExecuteFailure;
use crate::cas::CasState;
use crate::contention::ContentionMeasure;
use crate::help_queue::HelpQueue;
use crate::lockfree::NormalizedLockFree;
use crate::operation::OperationRecord;
use crate::operation::OperationRecordBox;
use crate::operation::OperationState;

const RETRY_THRESHOLD: usize = 2;

pub(crate) struct Shared<LF: NormalizedLockFree, const N: usize> {
  algorithm: LF,
  help: HelpQueue<LF, N>,
  free_ids: Mutex<Vec<usize>>,
}

pub(crate) struct WaitFreeSimulator<LF: NormalizedLockFree, const N: usize> {
  shared: Arc<Shared<LF, N>>,
  id: usize,
}

pub(crate) struct TooManyHandles;

impl<LF: NormalizedLockFree, const N: usize> WaitFreeSimulator<LF, N> {
  pub(crate) fn new(algorithm: LF) -> Self {
    assert_ne!(N, 0);
    Self {
      shared: Arc::new(Shared {
        algorithm,
        help: HelpQueue::new(),
        // NOTE: The Self we return has already claimed 0, therefore 1..
        free_ids: Mutex::new((1..N).collect()),
      }),
      id: 0,
    }
  }

  pub(crate) fn fork(&self) -> Result<Self, TooManyHandles> {
    if let Some(id) = self.shared.free_ids.lock().unwrap().pop() {
      Ok(Self {
        shared: Arc::clone(&self.shared),
        id,
      })
    } else {
      return Err(TooManyHandles);
    }
  }
}

impl<LF: NormalizedLockFree, const N: usize> Drop for WaitFreeSimulator<LF, N> {
  fn drop(&mut self) {
    self.shared.free_ids.lock().unwrap().push(self.id);
  }
}

impl<LF: NormalizedLockFree, const N: usize> WaitFreeSimulator<LF, N>
where
  for<'a> &'a LF::CommitDescriptor: IntoIterator<Item = &'a dyn VersionedCas>,
{
  fn cas_execute(
    &self,
    descriptors: &LF::CommitDescriptor,
    contention: &mut ContentionMeasure,
  ) -> Result<(), CasExecuteFailure> {
    for (i, cas) in descriptors.into_iter().enumerate() {
      match cas.state() {
        | CasState::Success => {
          cas.clear_bit();
        }
        | CasState::Failure => {
          return Err(CasExecuteFailure::CasFailed(i));
        }
        | CasState::Pending => {
          cas.execute(contention)?;
          if cas.has_modified_bit() {
            // XXX: Paper and code diverge here.
            cas.set_state(CasState::Success);
            cas.clear_bit();
          }
          if cas.state() != CasState::Success {
            cas.set_state(CasState::Failure);
            return Err(CasExecuteFailure::CasFailed(i));
          }
        }
      }
    }
    Ok(())
  }

  // Guarantees that on return, orb is no longer in help queue.
  fn help_op(&self, orb: &OperationRecordBox<LF>) {
    loop {
      let or = unsafe { &*orb.val.load(Ordering::SeqCst) };
      let updated_or = match &or.state {
        | OperationState::Completed(..) => {
          let _ = self.shared.help.try_remove_front(orb);
          return;
        }
        | OperationState::PreCas => {
          let cas_list = match self
            .shared
            .algorithm
            .generator(&or.input, &mut ContentionMeasure(0))
          {
            | Ok(cas_list) => cas_list,
            | Err(Contention) => continue,
          };
          Box::new(OperationRecord {
            owner: or.owner.clone(),
            input: or.input.clone(),
            state: OperationState::ExecuteCas(cas_list),
          })
        }
        | OperationState::ExecuteCas(cas_list) => {
          let outcome =
            match self.cas_execute(cas_list, &mut ContentionMeasure(0)) {
              | Ok(outcome) => Ok(outcome),
              | Err(CasExecuteFailure::CasFailed(i)) => Err(i),
              | Err(CasExecuteFailure::Contention) => continue,
            };
          Box::new(OperationRecord {
            owner: or.owner.clone(),
            input: or.input.clone(),
            state: OperationState::PostCas(cas_list.clone(), outcome),
          })
        }
        | OperationState::PostCas(cas_list, outcome) => {
          match self.shared.algorithm.wrap_up(
            *outcome,
            cas_list,
            &mut ContentionMeasure(0),
          ) {
            | Ok(Some(result)) => Box::new(OperationRecord {
              owner: or.owner.clone(),
              input: or.input.clone(),
              state: OperationState::Completed(result),
            }),
            | Ok(None) => {
              // We need to re-start from the generator.
              Box::new(OperationRecord {
                owner: or.owner.clone(),
                input: or.input.clone(),
                state: OperationState::PreCas,
              })
            }
            | Err(Contention) => {
              // Not up to us to re-start.
              continue;
            }
          }
        }
      };
      let updated_or = Box::into_raw(updated_or);

      if orb
        .val
        .compare_exchange_weak(
          or as *const OperationRecord<_> as *mut OperationRecord<_>,
          updated_or,
          Ordering::SeqCst,
          Ordering::Relaxed,
        )
        .is_err()
      {
        // Never got shared, so safe to drop.
        let _ = unsafe { Box::from_raw(updated_or) };
      }
    }
  }

  fn help_first(&self) {
    if let Some(help) = self.shared.help.peek() {
      self.help_op(unsafe { &*help });
    }
  }

  pub(crate) fn run(&self, op: LF::Input) -> LF::Output {
    let help = /* once in a while */ true;
    if help {
      self.help_first();
    }

    // fast path
    for retry in 0.. {
      let mut contention = ContentionMeasure(0);
      match self.shared.algorithm.fast_path(&op, &mut contention) {
        | Ok(result) => return result,
        | Err(Contention) => {}
      }

      if retry > RETRY_THRESHOLD {
        break;
      }
    }

    // slow path: ask for help.
    let orb = OperationRecordBox {
      val: AtomicPtr::new(Box::into_raw(Box::new(OperationRecord {
        owner: std::thread::current().id(),
        input: op,
        state: OperationState::PreCas,
      }))),
    };
    self.shared.help.enqueue(self.id, &orb);
    loop {
      let or = unsafe { &*orb.val.load(Ordering::SeqCst) };
      if let OperationState::Completed(t) = &or.state {
        break t.clone();
      } else {
        self.help_first();
      }
    }
  }
}
