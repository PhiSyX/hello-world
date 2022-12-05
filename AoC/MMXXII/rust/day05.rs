use core::fmt;
use core::str::FromStr;
use std::ops::Sub;

use mmxxii_rust::{filter_map_line, solved_output};

const PUZZLE: &str = include_str!("../puzzles/day05.txt");

const CRATE_CHAR_IDX: [usize; 9] = [1, 5, 9, 13, 17, 21, 25, 29, 33];

struct CrateStack {
    crates: Vec<char>,
}

impl FromStr for CraneOperation {
    type Err = std::num::ParseIntError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let parts = s.split_ascii_whitespace().collect::<Vec<_>>();

        if parts.len() != 6 {
            return "erreur".parse();
        }

        let total = parts[1].parse()?;

        let from = {
            let n: usize = parts[3].parse()?;
            n.sub(1)
        };

        let to = {
            let n: usize = parts[5].parse()?;
            n.sub(1)
        };

        Ok(Self { total, from, to })
    }
}

impl CrateStack {
    fn new(crates: Vec<char>) -> Self {
        Self { crates }
    }

    fn remove(&mut self, n: usize) -> Vec<char> {
        (0..n).filter_map(|_| self.crates.pop()).collect()
    }

    fn add(&mut self, crates: impl IntoIterator<Item = char>) {
        self.crates.extend(crates);
    }
}

struct CraneOperation {
    total: usize,
    from: usize,
    to: usize,
}

fn main() {
    println!("--- Day 5: Supply Stacks ---");

    let part01 = solve_part01(PUZZLE).expect("part 01");
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));
}

fn parse(
    input: &'static str,
) -> Option<(Vec<CrateStack>, impl Iterator<Item = CraneOperation>)> {
    let (crate_stacks_input, crane_operations) =
        input.split_once("\n\n")?;

    let crate_stacks_lines: Vec<_> =
        crate_stacks_input.lines().rev().skip(1).collect();

    let mut crate_stacks = Vec::with_capacity(CRATE_CHAR_IDX.len());

    for idx in CRATE_CHAR_IDX {
        let crates = crate_stacks_lines
            .iter()
            .filter_map(|line| line.chars().nth(idx))
            .filter(|ch| !ch.is_whitespace())
            .collect();

        crate_stacks.push(CrateStack::new(crates));
    }

    let crane_operations =
        filter_map_line(crane_operations, |line| line.parse().ok());

    Some((crate_stacks, crane_operations))
}

fn display_crates(list_crates: &[CrateStack]) -> impl fmt::Display {
    list_crates
        .iter()
        .filter_map(|s| s.crates.last())
        .collect::<String>()
}

fn solve_part01(input: &'static str) -> Option<impl fmt::Display> {
    let (mut crate_stacks, crane_operations) = parse(input)?;

    for crane_operation in crane_operations {
        let crates = crate_stacks
            .get_mut(crane_operation.from)?
            .remove(crane_operation.total);

        crate_stacks.get_mut(crane_operation.to)?.add(crates);
    }

    display_crates(&crate_stacks).into()
}
