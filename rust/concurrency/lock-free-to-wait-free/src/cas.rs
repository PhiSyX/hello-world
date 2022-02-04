use crate::contention::Contention;

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub(crate) enum CasState {
  Success,
  Failure,
  Pending,
}

pub(crate) struct CasByRcu<T> {
  pub(crate) version: u64,

  /// The value that will actually be CASed.
  pub(crate) value: T,
}

pub(crate) enum CasExecuteFailure {
  CasFailed(usize),
  Contention,
}

impl From<Contention> for CasExecuteFailure {
  fn from(_: Contention) -> Self {
    Self::Contention
  }
}
