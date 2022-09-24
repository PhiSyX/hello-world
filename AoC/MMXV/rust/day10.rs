use std::{iter::Peekable, ops::Range, str::Chars};

use mmxv_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day10.txt");

fn repeat(input: &str, times: Range<u8>) -> String {
    times
        .into_iter()
        .fold(String::from(input), |las, _| look_and_say(&las))
}

fn look_and_say(input: &str) -> String {
    let mut output = String::new();
    let mut chars = input.chars().peekable();

    while let Some(ch) = chars.next() {
        let next_ch = lookup_until(&mut chars, |nch| ch.ne(nch)) + 1;
        output.push_str(&next_ch.to_string());
        output.push_str(&ch.to_string());
    }

    output
}

fn lookup_until(
    chars: &mut Peekable<Chars>,
    predicate: impl Fn(&char) -> bool,
) -> usize {
    let mut counter = 0;
    while let Some(ch) = chars.peek() {
        if predicate(ch) {
            break;
        }
        chars.next();
        counter += 1;
    }
    counter
}

fn main() {
    println!("--- Day 10: Elves Look, Elves Say ---");

    println!("--- Part One ---");
    let part01 = solve_part01(PUZZLE);
    println!("\tYour puzzle answer is {}.", solved_output(part01).len());
}

fn solve_part01(input: &str) -> String {
    repeat(input, 0..40)
}
