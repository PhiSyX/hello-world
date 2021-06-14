use std::sync::atomic::AtomicPtr;

use crate::normalized_lockfree::NormalizedLockFree;

pub struct OperationRecordBox<LF: NormalizedLockFree> {
  pub val: AtomicPtr<OperationRecord<LF>>,
}

pub enum OperationState<LF: NormalizedLockFree> {
  PreCas,
  ExecuteCas(LF::Cases),
  PostCas(LF::Cases, Result<(), usize>),
  Completed(LF::Output),
}

pub struct OperationRecord<LF: NormalizedLockFree> {
  pub owner_tid: std::thread::ThreadId,
  pub input: LF::Input,
  pub state: OperationState<LF>,
}
