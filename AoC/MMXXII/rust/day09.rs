use core::fmt;
use core::str;
use std::collections::HashSet;

use mmxxii_rust::{filter_map_line, solved_output};

const PUZZLE: &str = include_str!("../puzzles/day09.txt");

#[derive(Debug, Clone, Copy)]
enum Direction {
    U,
    D,
    L,
    R,
}

impl str::FromStr for Direction {
    type Err = &'static str;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Ok(match s {
            | "U" => Self::U,
            | "D" => Self::D,
            | "L" => Self::L,
            | "R" => Self::R,
            | _ => return Err("Invalid Direction"),
        })
    }
}

#[derive(Debug, Clone, Copy)]
struct Move {
    direction: Direction,
    count: u16,
}

impl str::FromStr for Move {
    type Err = ();

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let (direction, count) = s.split_once(' ').ok_or(())?;
        direction
            .parse()
            .ok()
            .zip(count.parse().ok())
            .map(|(direction, count)| Self { direction, count })
            .ok_or(())
    }
}

#[derive(Debug, Default, Copy, Clone, PartialEq, Eq, Hash)]
struct Point(i16, i16);

fn new_tail_position(Point(hx, hy): Point, tail: &mut Point) {
    let (dx, dy) = (hx - tail.0, hy - tail.1);
    if dx.abs() > 1 || dy.abs() > 1 {
        tail.0 += dx.signum();
        tail.1 += dy.signum();
    }
}

fn main() {
    println!("--- Day 9: Rope Bridge ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));

    let part02 = solve_part02(PUZZLE);
    println!("--- Part Two ---");
    println!("\tYour puzzle answer is {}.", solved_output(part02));
}

fn solve<const N: usize>(input: &'static str) -> usize {
    let moves = filter_map_line(input, |line| line.parse::<Move>().ok());

    let mut tail_positions = HashSet::with_capacity(2000);
    tail_positions.insert(Point::default());

    let mut rope = [Point::default(); N];
    for Move { direction, count } in moves {
        'a: for _ in 0..count {
            match direction {
                | Direction::U => rope[0].1 += 1,
                | Direction::D => rope[0].1 -= 1,
                | Direction::R => rope[0].0 += 1,
                | Direction::L => rope[0].0 -= 1,
            }

            for i in 1..N {
                let last = rope[i];

                new_tail_position(rope[i - 1], &mut rope[i]);

                if rope[i] == last {
                    continue 'a;
                }
            }

            tail_positions.insert(rope[N - 1]);
        }
    }

    tail_positions.len()
}

fn solve_part01(input: &'static str) -> impl fmt::Display {
    solve::<2>(input)
}

fn solve_part02(input: &'static str) -> impl fmt::Display {
    solve::<10>(input)
}
