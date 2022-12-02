use core::fmt;
use core::str::FromStr;
use std::ops::Add;

use mmxxii_rust::{filter_map_line, solved_output};

const PUZZLE: &str = include_str!("../puzzles/day02.txt");

#[derive(Debug)]
#[derive(Copy, Clone)]
enum Move {
    Rock,
    Paper,
    Scissors,
}

impl FromStr for Move {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Ok(match s {
            | "A" | "X" => Self::Rock,
            | "B" | "Y" => Self::Paper,
            | "C" | "Z" => Self::Scissors,
            | move_s => return Err(format!("invalid move : '{move_s}'")),
        })
    }
}

#[derive(Debug)]
struct Game(Move, Move);

impl Game {
    fn new(g: (Move, Move)) -> Self {
        Self(g.0, g.1)
    }

    fn score(self) -> usize {
        self.1 + self.0
    }
}

impl Add for Move {
    type Output = usize;

    fn add(self, rhs: Self) -> Self::Output {
        let s = self as usize + 1;

        let o = match (self, rhs) {
            | (Self::Rock, Self::Rock)
            | (Self::Paper, Self::Paper)
            | (Self::Scissors, Self::Scissors) => 3,

            | (Self::Rock, Self::Scissors)
            | (Self::Paper, Self::Rock)
            | (Self::Scissors, Self::Paper) => 6,

            | (Self::Paper, Self::Scissors)
            | (Self::Scissors, Self::Rock)
            | (Self::Rock, Self::Paper) => 0,
        };

        o + s
    }
}

fn main() {
    println!("--- Day 2: Rock Paper Scissors ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));
}

fn solve_part01(input: &'static str) -> impl fmt::Display {
    filter_map_line(input, |line| {
        line.split_once(' ')
            .and_then(|(a, b)| {
                a.parse::<Move>().ok().zip(b.parse::<Move>().ok())
            })
            .map(Game::new)
    })
    .map(Game::score)
    .sum::<usize>()
}
