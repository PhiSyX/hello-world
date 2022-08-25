use crate::contention::Contention;
use crate::contention::ContentionMeasure;

pub(crate) trait NormalizedLockFree {
  type Input: Clone;
  type Output: Clone;
  type CommitDescriptor: Clone;

  fn generator(
    &self,
    op: &Self::Input,
    contention: &mut ContentionMeasure,
  ) -> Result<Self::CommitDescriptor, Contention>;
  fn wrap_up(
    &self,
    executed: Result<(), usize>,
    performed: &Self::CommitDescriptor,
    contention: &mut ContentionMeasure,
  ) -> Result<Option<Self::Output>, Contention>;

  fn fast_path(
    &self,
    op: &Self::Input,
    contention: &mut ContentionMeasure,
  ) -> Result<Self::Output, Contention>;
}
