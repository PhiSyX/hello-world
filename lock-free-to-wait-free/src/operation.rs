use std::sync::atomic::AtomicPtr;

use crate::lockfree::NormalizedLockFree;
use crate::node::Node;

pub(crate) struct OpDesc<T> {
  pub(crate) phase: Option<u64>,
  pub(crate) pending: bool,
  pub(crate) enqueue: bool,
  pub(crate) node: Option<*mut Node<T>>,
}

pub(crate) struct OperationRecordBox<LF: NormalizedLockFree> {
  pub(crate) val: AtomicPtr<OperationRecord<LF>>,
}
pub(crate) enum OperationState<LF: NormalizedLockFree> {
  PreCas,
  ExecuteCas(LF::CommitDescriptor),
  PostCas(LF::CommitDescriptor, Result<(), usize>),
  Completed(LF::Output),
}

pub(crate) struct OperationRecord<LF: NormalizedLockFree> {
  pub(crate) owner: std::thread::ThreadId,
  pub(crate) input: LF::Input,
  pub(crate) state: OperationState<LF>,
}
