use std::io;

fn data_types() {
  scalar_types();

  compound_types();
}

//? A scalar type represents a single value. Rust has four primary scalar types:
//?   integers, floating-point numbers, Booleans, and characters.
fn scalar_types() {
  integer_type();
  floating_type();
  numeric_operations();
  bool_type();
  char_type();
}

//? Integer types
fn integer_type() {
  let d = 32; // i32 default ?
  let w = 9u8;
  let x = 256 as u16;
  let y: u32 = 100_000_000;
  let z: u64 = 100_000_000_000;
  dbg!(d, w, x, y, z);
}

//? Floating-Point Types
fn floating_type() {
  let x = 2.0; // f64
  let y: f32 = 3.0; // f32
  dbg!(x, y);
}

//? Numeric Operations
fn numeric_operations() {
  let sum = 5 + 10;
  let difference = 95.5 - 4.3;
  let product = 4 * 30;
  let quotient = 56.7 / 32.2;
  let remainder = 43 % 5;
  dbg!(sum, difference, product, quotient, remainder);
}

//? The Boolean Type
fn bool_type() {
  let t = true;
  let f: bool = false; // with explicit type annotation
  dbg!(t, f);
}

//? The Character Type
fn char_type() {
  let c = 'z';
  let z = 'ℤ';
  let heart_eyed_cat = '😻';
  dbg!(c, z, heart_eyed_cat);
}







//? Compound types can group multiple values into one type.
//? Rust has two primitive compound types: tuples and arrays.
fn compound_types() {
  tuple_type();
  array_type();
}

//? The Tuple Type
fn tuple_type() {
  let x: (i32, f64, u8) = (500, 6.4, 1);

  let five_hundred = x.0;

  let six_point_four = x.1;

  let one = x.2;

  dbg!(five_hundred, six_point_four, one);
}

//? The Array Type
fn array_type() {
  let months = [
    "January", "February", "March",
    "April", "May", "June",
    "July", "August", "September",
    "October", "November", "December"
  ];

  //? Accessing Array Elements
  let first = months[0];
  let second = months[1];

  dbg!(months, first, second);

  //? Invalid Array Element Access

  println!("Please enter an array index.");

  let mut index = String::new();

  io::stdin()
      .read_line(&mut index)
      .expect("Failed to read line");

  let index: usize = index
      .trim()
      .parse()
      .expect("Index entered was not a number");

  let element = months[index]; // Bug à partir de l'index 12

  println!(
      "The value of the element at index {} is: {}",
      index, element
  );
}



pub fn section_2() {
  data_types();
}
