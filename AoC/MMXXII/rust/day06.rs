use core::fmt;
use core::ops::Add;

use itertools::*;
use mmxxii_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day06.txt");

fn main() {
    println!("--- Day 6: Tuning Trouble ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));

    let part02 = solve_part02(PUZZLE);
    println!("--- Part Two ---");
    println!("\tYour puzzle answer is {}.", solved_output(part02));
}

fn solve<const N: usize>(input: &'static str) -> usize {
    input
        .chars()
        .collect::<Vec<_>>()
        .windows(N)
        .position(|chars| chars.iter().sorted().dedup().count() == N)
        .unwrap_or_default()
        .add(N)
}

fn solve_part01(input: &'static str) -> impl fmt::Display {
    solve::<4>(input)
}

fn solve_part02(input: &'static str) -> impl fmt::Display {
    solve::<14>(input)
}
