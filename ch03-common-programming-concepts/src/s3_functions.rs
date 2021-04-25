//? Function Parameters
fn another_function(x: i32, y: i32) {
  dbg!(x);
  dbg!(y);
}

//? Function Bodies Contain Statements and Expressions
fn function_body() {
  let x = 5;

  let y = {
    let x = 3;
    x + 1
  };

  dbg!(y);
}

//? Functions with Return Values
fn return_value() {
  let x = five();
  dbg!(x);

  let x = plus_one(5);
  dbg!(x);
}

fn five() -> i32 { 5 }
fn plus_one(x: i32) -> i32 { x + 1 }


pub fn section_3() {
  another_function(5, 6);
  function_body();
  return_value();
}

