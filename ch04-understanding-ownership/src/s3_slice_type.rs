fn first_word(s: &String) -> usize {
  let bytes = s.as_bytes();

  for (i, &item) in bytes.iter().enumerate() {
    if item == b' ' {
      return i;
    }
  }

  s.len()
}

fn first_word_slice(s: &str) -> &str {
  let bytes = s.as_bytes();

  for (i, &item) in bytes.iter().enumerate() {
    if item == b' ' {
      return &s[0..i];
    }
  }

  &s[..]
}

pub fn section_3() {
  let mut s1 = String::from("hello");
  let word = first_word(&s1);

  s1.clear(); // this empties the String, making it equal to ""

  // word still has the value 5 here, but there's no more string that
  // we could meaningfully use the value 5 with. word is now totally invalid!

  dbg!(word);

  let my_string = String::from("hello world");

  // first_word_slice works on slices of `String`s
  let word = first_word_slice(&my_string[..]);

  let my_string_literal = "hello world";
  dbg!(word);

  // first_word_slice works on slices of string literals
  let word = first_word_slice(&my_string_literal[..]);
  dbg!(word);

  // Because string literals *are* string slices already,
  // this works too, without the slice syntax!
  let word = first_word_slice(my_string_literal);
  dbg!(word);
}
