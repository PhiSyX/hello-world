//? Variables and Mutability
fn variable_mut() {
  let mut x = 5;
  println!("The value of x is: {}", x);
  x = 6;
  println!("The value of x is: {}", x);
}

const MAX_POINTS: u32 = 100_000;

//? Differences Between Variables and Constants
fn constants() {
  println!("The value of MAX_POINTS is: {}", MAX_POINTS);
}

//? Shadowing
fn shadowing() {
  let x = 5;
  let x = x + 1;
  let x = x * 2;

  println!("The value of x is: {}", x);
}

pub fn section_1() {
  variable_mut();
  constants();
  shadowing();
}
