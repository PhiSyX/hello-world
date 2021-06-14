use std::ops::Index;

use crate::contention_measure::Contention;
use crate::contention_measure::ContentionMeasure;

pub trait CasDescriptor {
  fn execute(&self) -> Result<(), ()>;
}

pub trait CasDescriptors<D>: Index<usize, Output = D>
where
  D: CasDescriptor,
{
  fn len(&self) -> usize;
}

pub trait NormalizedLockFree {
  type Input: Clone;
  type Output: Clone;
  type Cas: CasDescriptor;
  type Cases: CasDescriptors<Self::Cas> + Clone;

  fn generator(
    &self,
    op: &Self::Input,
    contention: &mut ContentionMeasure,
  ) -> Result<Self::Cases, Contention>;

  fn wrap_up(
    &self,
    executed: Result<(), usize>,
    performed: &Self::Cases,
    contention: &mut ContentionMeasure,
  ) -> Result<Option<Self::Output>, Contention>;
}
