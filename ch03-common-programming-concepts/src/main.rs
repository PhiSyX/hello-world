mod s01_variables_and_mutability;
use std::io;

use s01_variables_and_mutability::section_1;

mod s02_data_types;
use s02_data_types::section_2;

mod s03_how_functions_work;
use s03_how_functions_work::section_3;

mod s05_control_flow;
use s05_control_flow::section_5;

fn main() {
  section_1();
  section_2();
  section_3();
  section_5();

  summary();
}

const LYRICS: [&str; 12] = [
  "a partridge in a pear tree",
  "two turtle doves",
  "three french hens",
  "four calling birds",
  "FIVE GOLD RINGS",
  "six geese a-laying",
  "seven swans a-swimming",
  "eight maids a-milking",
  "nine ladies dancing",
  "ten lords a-leaping",
  "eleven pipers piping",
  "twelve drummers drumming",
];

fn summary() {
  //? Convert temperatures between Fahrenheit and Celsius.
  println!("Entrer une température Fahrenheit");
  let mut fahrenheit = String::new();
  io::stdin().read_line(&mut fahrenheit).expect("Erreur");
  let fahrenheit = fahrenheit.trim().parse::<f32>().unwrap();
  let celcius = (fahrenheit - 32.0) * (5.0 / 9.0);
  println!("La température Fahrenheit {}° en Celcius est {:.2}°", fahrenheit, celcius);

  //? Generate the nth Fibonacci number.
  println!("Entre un nombre: (fibonacci)");
  let mut n = String::new();
  io::stdin().read_line(&mut n).expect("Erreur");
  let n = n.trim().parse::<i32>().expect("Valeur invalide");

  println!("Fibonacci de {} = {};", n, simple_fib(n));

  //? Print the lyrics to the Christmas carol “The Twelve Days of Christmas,” taking advantage of the repetition in the song.
  for day in 0..LYRICS.len() {
    let suffix = match day + 1 {
      | 1 => "st",
      | 2 => "nd",
      | 3 => "rd",
      | _ => "th",
    };

    println!();
    println!("On the {}{} day of xmas my true love gave to me:", day + 1, suffix);

    for i in (0..day + 1).rev() {
      if day > 0 && i == 0 { print!("And "); }
      println!("{}", LYRICS[i]);
    }
  }
}

fn simple_fib(n: i32) -> i32 {
  if n <= 0 { panic!("Valeur invalide"); }

  if n == 1 { return n; }

	let mut s = 0;
	let mut l = 0;
	let mut c = 1;

	for _i in 1..n {
		s = l + c;
		l = c;
		c = s;
	}

	s
}
