use core::fmt;
use std::{collections::HashSet, ops::Div};

use mmxxii_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day03.txt");

fn main() {
    println!("--- Day 3: Rucksack Reorganization ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));
}

fn solve_part01(input: &'static str) -> impl fmt::Display {
    input
        .lines()
        .flat_map(|line| {
            let (l, r) = line.split_at(line.len().div(2));
            let [l, r] = [l, r].map(|p| p.chars().collect::<HashSet<_>>());
            l.intersection(&r).copied().collect::<Vec<_>>()
        })
        .filter_map(|ch| {
            Some(match ch {
                | 'a'..='z' => (ch as u8 - b'a' + 1) as usize,
                | 'A'..='Z' => (ch as u8 - b'A' + 27) as usize,
                | _ => return None,
            })
        })
        .sum::<usize>()
}
