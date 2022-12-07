use core::fmt;
use std::ops::Add;

use itertools::*;
use mmxxii_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day06.txt");

fn main() {
    println!("--- Day 6: Tuning Trouble ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));
}

fn solve_part01(input: &'static str) -> impl fmt::Display {
    input
        .chars()
        .collect::<Vec<_>>()
        .windows(4)
        .position(|chars| chars.iter().sorted().dedup().count() == 4)
        .unwrap_or_default()
        .add(4)
}
