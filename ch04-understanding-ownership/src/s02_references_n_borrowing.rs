fn ref_bow(s1: &String) {
  let len = calculate_length(&s1);

  println!("The length of '{}' is {}.", s1, len);
}

fn calculate_length(s: &String) -> usize { // s is a reference to a String
  s.len()
} // Here, s goes out of scope. But because it does not have ownership of what
  // it refers to, nothing happens.

fn change(some_string: &mut String) {
  some_string.push_str(", world");
}


fn first_word(s: &String) -> usize {
  let bytes = s.as_bytes();

  for (i, &item) in bytes.iter().enumerate() {
    if item == b' ' {
      return i;
    }
  }

  s.len()
}


pub fn section_2() {
  let s1 = String::from("hello");

  ref_bow(&s1);

  let mut s2 = String::from("hello");

  change(&mut s2);

  let s3 = first_word(&s2);

  dbg!(s3);
}
