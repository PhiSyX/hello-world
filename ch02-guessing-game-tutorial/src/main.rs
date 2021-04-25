use std::cmp::Ordering;
use std::io;

use rand::Rng;

fn main() {
  // processing_guess();
  // generating_random_number()
  generating_random_number_loop()
}

#[allow(dead_code)]
//? Processing a Guess
fn processing_guess() {
  println!("Guess the number!");

  println!("Please input your guess.");

  let mut guess = String::new();

  io::stdin()
    .read_line(&mut guess)
    .expect("Failed to read line");

  println!("You guessed: {}", guess);
}

#[allow(dead_code)]
//? Generating a Random Number
fn generating_random_number() {
  println!("Guess the number!");

  let secret_number = rand::thread_rng()
    // .gen_range(1, 101); //? ancienne version de la crate
    .gen_range(1..101);    //? nouvelle version de la crate

  println!("The secret number is: {}", secret_number);

  println!("Please input your guess.");

  let mut guess = String::new();

  io::stdin()
    .read_line(&mut guess)
    .expect("Failed to read line");

  let guess: u32 = guess.trim().parse().expect("Please type a number!");

  println!("You guessed: {}", guess);

  match guess.cmp(&secret_number) {
    Ordering::Less => println!("Too small!"),
    Ordering::Greater => println!("Too big!"),
    Ordering::Equal => println!("You win!"),
  }
}

//? Allowing Multiple Guesses with Looping
fn generating_random_number_loop() {
  println!("Guess the number!");

  let secret_number = rand::thread_rng()
    // .gen_range(1, 101); //? ancienne version de la crate
    .gen_range(1..101);    //? nouvelle version de la crate

  println!("The secret number is: {}", secret_number);

  loop {
    println!("Please input your guess.");

    let mut guess = String::new();

    io::stdin()
      .read_line(&mut guess)
      .expect("Failed to read line");

    //? Handling Invalid Input
    let guess: u32 = match guess.trim().parse() {
      Ok(num) => num,
      Err(_)  => continue,
    };

    println!("You guessed: {}", guess);

    match guess.cmp(&secret_number) {
      Ordering::Less => println!("Too small!"),
      Ordering::Greater => println!("Too big!"),
      Ordering::Equal => {
        println!("You win!");
        break; // ? Quitting After a Correct Guess
      },
    }
  }
}
