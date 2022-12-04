use core::fmt;

use mmxxii_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day04.txt");

struct Range {
    start: i32,
    end: i32,
}

impl std::str::FromStr for Range {
    type Err = std::num::ParseIntError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let Some((l, r)) = s.split_once('-') else {
            return "erreur".parse();
        };

        let start = l.parse()?;
        let end = r.parse()?;

        Ok(Range { start, end })
    }
}

impl Range {
    fn contains(&self, other: &Range) -> bool {
        self.start <= other.start && self.end >= other.end
    }
}

fn main() {
    println!("--- Day 4: Camp Cleanup ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));
}

fn solve_part01(input: &'static str) -> impl fmt::Display {
    input
        .lines()
        .filter_map(|line| {
            let Some((l, r)) = line.split_once(',') else {
                return None;
            };
            l.parse::<Range>().ok().zip(r.parse::<Range>().ok())
        })
        .fold(0, |mut acc, (r1, r2)| {
            if r1.contains(&r2) || r2.contains(&r1) {
                acc += 1;
            }
            acc
        })
}
