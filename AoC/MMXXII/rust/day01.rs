use core::fmt;

use mmxxii_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day01.txt");

type U = usize;
fn solve_part01(input: &'static str) -> impl fmt::Display {
    input
        .split_terminator("\n\n")
        .map(|e| e.lines().filter_map(|l| l.parse::<U>().ok()).sum::<U>())
        .max()
        .unwrap_or_default()
}

fn main() {
    println!("--- Day 1: Calorie Counting ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));
}
