pub struct ContentionMeasure(pub usize);

const CONTENTION_THRESHOLD: usize = 2;

pub struct Contention;

impl ContentionMeasure {
  pub fn detected(&mut self) -> Result<(), Contention> {
    self.0 += 1;
    if self.0 < CONTENTION_THRESHOLD {
      Ok(())
    } else {
      Err(Contention)
    }
  }

  pub fn use_slow_path(&self) -> bool {
    self.0 > CONTENTION_THRESHOLD
  }
}
