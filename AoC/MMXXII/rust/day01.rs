use core::fmt;

use mmxxii_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day01.txt");

type U = usize;

trait UsizeSorted {
    fn sorted(self) -> std::vec::IntoIter<U>
    where
        Self: Sized,
        Self: Iterator<Item = U>,
    {
        let mut v = Vec::from_iter(self);
        v.sort();
        v.into_iter()
    }
}

impl<UsizeIterator> UsizeSorted for UsizeIterator {}

fn solve_part01(input: &'static str) -> impl fmt::Display {
    input
        .split_terminator("\n\n")
        .map(|e| e.lines().filter_map(|l| l.parse::<U>().ok()).sum::<U>())
        .max()
        .unwrap_or_default()
}

fn solve_part02(input: &'static str) -> impl fmt::Display {
    input
        .split_terminator("\n\n")
        .map(|e| e.lines().filter_map(|l| l.parse::<U>().ok()).sum::<U>())
        .sorted()
        .rev()
        .take(3)
        .sum::<usize>()
}

fn main() {
    println!("--- Day 1: Calorie Counting ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));

    let part02 = solve_part02(PUZZLE);
    println!("--- Part Two ---");
    println!("\tYour puzzle answer is {}.", solved_output(part02));
}
