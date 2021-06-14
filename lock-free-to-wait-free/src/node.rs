use std::sync::atomic::AtomicPtr;

pub(crate) struct Node<T> {
  pub(crate) value: Option<T>,
  pub(crate) next: AtomicPtr<Self>,
  pub(crate) enq_id: Option<usize>,
}

impl<T> Node<T> {
  pub(crate) fn new(value: T, enq_id: usize) -> *mut Self {
    Box::into_raw(Box::new(Self {
      value: Some(value),
      next: AtomicPtr::new(std::ptr::null_mut()),
      enq_id: Some(enq_id),
    }))
  }

  pub(crate) fn sentinel() -> Self {
    Self {
      value: None,
      next: AtomicPtr::new(std::ptr::null_mut()),
      enq_id: None,
    }
  }
}
