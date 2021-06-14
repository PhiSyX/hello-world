use std::sync::atomic::AtomicPtr;
use std::sync::atomic::Ordering;

use crate::contention_measure::ContentionMeasure;
use crate::help::HelpQueue;
use crate::normalized_lockfree::CasDescriptor;
use crate::normalized_lockfree::CasDescriptors;
use crate::normalized_lockfree::NormalizedLockFree;
use crate::operation_record::OperationRecord;
use crate::operation_record::OperationRecordBox;
use crate::operation_record::OperationState;

const RETRY_THRESHOLD: usize = 2;

pub struct WaitFreeSimulator<LF: NormalizedLockFree> {
  algorithm: LF,
  help: HelpQueue<LF>,
}

impl<LF: NormalizedLockFree> WaitFreeSimulator<LF>
where
  OperationRecord<LF>: Clone,
{
  fn cas_execute(
    &self,
    descriptors: &LF::Cases,
    contention: &mut ContentionMeasure,
  ) -> Result<(), usize> {
    let len = descriptors.len();
    for i in 0..len {
      // TODO: Check if already completed.
      // implement Figure 5.
      if descriptors[i].execute().is_err() {
        contention.detected();
        return Err(i);
      }
    }

    Ok(())
  }

  fn help_first(&self) {
    if let Some(help) = self.help.peek() {
      self.help_op(unsafe { &*help });
    }
  }

  // Guarantees that on return, opre_box is no longer in help queue.
  fn help_op(&self, opre_box: &OperationRecordBox<LF>) {
    loop {
      let opre = unsafe { &*opre_box.val.load(Ordering::SeqCst) };

      let updated_opre = match &opre.state {
        | OperationState::PreCas => {
          let cas_list = match self
            .algorithm
            .generator(&opre.input, &mut ContentionMeasure(0))
          {
            | Ok(cast_list) => cast_list,
            | Err(_) => continue,
          };

          Box::new(OperationRecord {
            owner_tid: opre.owner_tid,
            input: opre.input.clone(),
            state: OperationState::ExecuteCas(cas_list),
          })
        }
        | OperationState::ExecuteCas(ref cas_list) => {
          let outcome = self.cas_execute(cas_list, &mut ContentionMeasure(0));

          Box::new(OperationRecord {
            owner_tid: opre.owner_tid,
            input: opre.input.clone(),
            state: OperationState::PostCas(cas_list.clone(), outcome),
          })
        }
        | OperationState::PostCas(cas_list, outcome) => {
          match self.algorithm.wrap_up(
            *outcome,
            cas_list,
            &mut ContentionMeasure(0),
          ) {
            | Ok(Some(result)) => Box::new(OperationRecord {
              owner_tid: opre.owner_tid,
              input: opre.input.clone(),
              state: OperationState::Completed(result),
            }),
            | Ok(None) => Box::new(OperationRecord {
              owner_tid: opre.owner_tid,
              input: opre.input.clone(),
              state: OperationState::PreCas,
            }),
            | Err(Contention) => {
              // Not up to us to restart
              continue;
            }
          }
        }
        | OperationState::Completed(_) => {
          let _ = self.help.try_remove_front(opre_box);
          return;
        }
      };

      let updated_opre = Box::into_raw(updated_opre);

      if opre_box
        .val
        .compare_exchange_weak(
          opre as *const OperationRecord<_> as *mut OperationRecord<_>,
          updated_opre,
          Ordering::SeqCst,
          Ordering::Relaxed,
        )
        .is_err()
      {
        // never got shared, so sage to drop
        let _ = unsafe { Box::from_raw(updated_opre) };
      }
    }
  }

  pub fn run(&self, opre: LF::Input) -> LF::Output {
    let help = false;
    if help {
      self.help_first()
    }

    // fast path
    for retry in 0.. {
      let mut contention = ContentionMeasure(0);

      let cases = self.algorithm.generator(&opre, &mut contention);

      if contention.use_slow_path() {
        break;
      }

      let result = self.cas_execute(&cases, &mut contention);

      if contention.use_slow_path() {
        break;
      }

      match self.algorithm.wrap_up(result, &cases, &mut contention) {
        | Ok(outcome) => return outcome,
        | Err(_) => {}
      }

      if contention.use_slow_path() {
        break;
      }

      if retry > RETRY_THRESHOLD {
        break;
      }
    }

    // slow-paht: ask for help.
    let opre_box = OperationRecordBox {
      val: AtomicPtr::new(Box::into_raw(Box::new(OperationRecord {
        owner_tid: std::thread::current().id(),
        input: opre,
        state: OperationState::PreCas,
      }))),
    };

    self.help.enqueue(&opre_box);

    loop {
      // Safety: ???
      let opre = unsafe { &*opre_box.val.load(Ordering::SeqCst) };

      if let OperationState::Completed(t) = &opre.state {
        break t.clone();
      } else {
        self.help_first();
      }
    }
  }
}
