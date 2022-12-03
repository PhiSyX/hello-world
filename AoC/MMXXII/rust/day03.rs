#![feature(iter_array_chunks)]

use core::fmt;
use core::ops::Div;
use std::collections::HashSet;

use mmxxii_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day03.txt");

fn main() {
    println!("--- Day 3: Rucksack Reorganization ---");

    let part01 = solve_part01(PUZZLE);
    println!("--- Part One ---");
    println!("\tYour puzzle answer is {}.", solved_output(part01));

    let part02 = solve_part02(PUZZLE);
    println!("--- Part Two ---");
    println!("\tYour puzzle answer is {}.", solved_output(part02));
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

fn solve_part02(input: &'static str) -> impl fmt::Display {
    input
        .lines()
        .map(|line| line.chars().collect::<HashSet<_>>())
        .array_chunks::<3>()
        .flat_map(|bags| {
            let mut bags = bags.iter();

            bags.next()
                .and_then(|b| {
                    bags.next().map(|i| {
                        b.intersection(&i).copied().collect::<HashSet<_>>()
                    })
                })
                .and_then(|b| {
                    bags.next().map(|i| {
                        b.intersection(&i).copied().collect::<Vec<_>>()
                    })
                })
                .unwrap()
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
