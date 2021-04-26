pub fn section_2() {
  let mut s = String::new();

  let data = "initial contents";
  let s = data.to_string();
  let s = "initial contents".to_string();
  let s = String::from("initial contents");

  let hello = String::from("السلام عليكم");
  let hello = String::from("Dobrý den");
  let hello = String::from("Hello");
  let hello = String::from("שָׁלוֹם");
  let hello = String::from("नमस्ते");
  let hello = String::from("こんにちは");
  let hello = String::from("안녕하세요");
  let hello = String::from("你好");
  let hello = String::from("Olá");
  let hello = String::from("Здравствуйте");
  let hello = String::from("Hola");

  let mut s1 = String::from("foo");
  let s2 = "bar";
  s1.push_str(s2);
  println!("s2 is {}", s2);

  let mut s = String::from("lo");
  s.push('l');
  dbg!(s);

  for c in "नमस्ते".chars() {
    dbg!(c);
  }

  for b in "नमस्ते".bytes() {
    dbg!(b);
  }
}
