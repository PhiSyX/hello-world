pub fn index_lt(idx: usize, arr: &[u8]) -> Option<u8> {
    if idx < arr.len() {
        unsafe { Some(*arr.get_unchecked(idx)) }
    } else {
        None
    }
}

pub fn index_le(idx: usize, arr: &[u8]) -> Option<u8> {
    if idx <= arr.len() {
        unsafe { Some(*arr.get_unchecked(idx)) }
    } else {
        None
    }
}
