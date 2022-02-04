
fn if_elseif_expression() {
  let number = 22;
  let expression = number < 3;

  if expression {
    println!("condition was true");
  } else {
    println!("condition was false");
  }

  let number = 6;

  if number % 4 == 0 {
    println!("number is divisible by 4");
  } else if number % 3 == 0 {
    println!("number is divisible by 3");
  } else if number % 2 == 0 {
    println!("number is divisible by 2");
  } else {
    println!("number is not divisible by 4, 3, or 2");
  }
}

//? Using if in a let Statement
fn if_let_statement() {
  let condition = true;
  let number = if condition { 5 } else { 6 };
  dbg!(number);
}

//? Returning Values from Loops
fn values_from_loops() {
  let mut counter = 0;
  let result = loop {
    counter += 1;

    if counter == 10 {
      break counter * 2;
    }
  };

  dbg!(result);
}

//? Conditional Loops with while
fn conditional_while() {
  let mut number = 3;

  while number != 0 {
    dbg!(number);

    number -= 1;
  }

  println!("LIFTOFF!!!");
}

//? Looping Through a Collection with for
fn collection_for() {
  let a = [10, 20, 30, 40, 50];

  for element in a.iter() {
    dbg!(element);
  }

  for number in (1..4).rev() {
    dbg!(number);
  }

  println!("LIFTOFF!!!");
}

pub fn section_5() {
  if_elseif_expression();
  if_let_statement();

  values_from_loops();
  conditional_while();

  collection_for();
}

