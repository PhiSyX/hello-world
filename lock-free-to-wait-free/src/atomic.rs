use std::sync::atomic::AtomicPtr;
use std::sync::atomic::Ordering;

use crate::cas::CasByRcu;
use crate::cas::CasState;
use crate::contention::Contention;
use crate::contention::ContentionMeasure;

pub(crate) struct Atomic<T>(AtomicPtr<CasByRcu<T>>);

pub(crate) trait VersionedCas {
  fn execute(
    &self,
    contention: &mut ContentionMeasure,
  ) -> Result<bool, Contention>;
  fn has_modified_bit(&self) -> bool;
  fn clear_bit(&self) -> bool;
  fn state(&self) -> CasState;
  fn set_state(&self, new: CasState);
}

impl<T> Atomic<T>
where
  T: PartialEq + Eq,
{
  pub(crate) fn new(initial: T) -> Self {
    Self(AtomicPtr::new(Box::into_raw(Box::new(CasByRcu {
      version: 0,
      value: initial,
    }))))
  }

  fn get(&self) -> *mut CasByRcu<T> {
    self.0.load(Ordering::SeqCst)
  }

  pub(crate) fn with<F, R>(&self, f: F) -> R
  where
    F: FnOnce(&T, u64) -> R,
  {
    // Safety: this is safe because we never deallocate.
    let this = unsafe { &*self.get() };
    f(&this.value, this.version)
  }

  pub(crate) fn set(&self, value: T) {
    let this_ptr = self.get();
    // Safety: this is safe because we never deallocate.
    let this = unsafe { &*this_ptr };
    if this.value != value {
      self.0.store(
        Box::into_raw(Box::new(CasByRcu {
          version: this.version + 1,
          value,
        })),
        Ordering::SeqCst,
      );
    }
  }

  pub(crate) fn compare_and_set(
    &self,
    expected: &T,
    value: T,
    contention: &mut ContentionMeasure,
    version: Option<u64>,
  ) -> Result<bool, Contention> {
    let this_ptr = self.get();
    // Safety: this is safe because we never deallocate.
    let this = unsafe { &*this_ptr };
    if &this.value == expected {
      if let Some(v) = version {
        if v != this.version {
          contention.detected()?;
          return Ok(false);
        }
      }

      if expected == &value {
        Ok(true)
      } else {
        let new_ptr = Box::into_raw(Box::new(CasByRcu {
          version: this.version + 1,
          value,
        }));
        match self.0.compare_exchange(
          this_ptr,
          new_ptr,
          Ordering::SeqCst,
          Ordering::Relaxed,
        ) {
          | Ok(_) => Ok(true),
          | Err(_current) => {
            // Safety: the Box was never shared.
            let _ = unsafe { Box::from_raw(new_ptr) };
            contention.detected()?;
            Ok(false)
          }
        }
      }
    } else {
      Ok(false)
    }
  }
}
