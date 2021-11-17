pub(crate) struct Contention;

const CONTENTION_THRESHOLD: usize = 2;

pub(crate) struct ContentionMeasure(pub(crate) usize);

impl ContentionMeasure {
  pub(crate) fn detected(&mut self) -> Result<(), Contention> {
    self.0 += 1;
    if self.0 < CONTENTION_THRESHOLD {
      Ok(())
    } else {
      Err(Contention)
    }
  }

  pub(crate) fn use_slow_path(&self) -> bool {
    self.0 > CONTENTION_THRESHOLD
  }
}
