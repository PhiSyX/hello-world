use std::marker::PhantomData;

use crate::normalized_lockfree::NormalizedLockFree;
use crate::operation_record::OperationRecordBox;

// A wait-free queue
pub struct HelpQueue<LF: NormalizedLockFree> {
  _o: PhantomData<LF>,
}

impl<LF: NormalizedLockFree> HelpQueue<LF> {
  // TODO: implement based on appendix A
  pub fn enqueue(&self, help: *const OperationRecordBox<LF>) {}

  pub fn peek(&self) -> Option<*const OperationRecordBox<LF>> {
    None
  }

  pub fn try_remove_front(
    &self,
    front: *const OperationRecordBox<LF>,
  ) -> Result<(), ()> {
    Err(())
  }
}
