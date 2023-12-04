use std::collections::{HashMap, HashSet};
use std::ops::{Add, AddAssign};
use std::str::FromStr;

use mmxxiii_rust::{parse_line, solved_output};

const PUZZLE: &str = include_str!("../puzzles/day04.txt");

struct Card
{
	id: usize,
	winning: HashSet<usize>,
	numbers: Vec<usize>,
}

impl Card
{
	fn contains_winning_numbers<'a>(&'a self) -> impl Iterator<Item = &'a usize>
	{
		self.numbers.iter().filter(|number| self.winning.contains(number))
	}
}

impl FromStr for Card
{
	type Err = &'static str;

	fn from_str(s: &str) -> Result<Self, Self::Err>
	{
		let s = s.trim_start_matches("Card ").trim_start();
		let (raw_id, raw_wn) = s.split_once(':').ok_or("Pourquoi?!")?;
		let id = raw_id.parse().map_err(|_| "Pourquoi?!")?;
		let (raw_winning, raw_numbers) = raw_wn.split_once('|').ok_or("Pourquoi?!")?;

		let winnings = raw_winning
			.split_ascii_whitespace()
			.filter_map(|number| number.parse().ok());

		let numbers = raw_numbers
			.split_ascii_whitespace()
			.filter_map(|number| number.parse().ok());

		Ok(Self {
			id,
			winning: winnings.collect(),
			numbers: numbers.collect(),
		})
	}
}

fn solve_part01(input: &'static str) -> usize
{
	parse_line::<Card>(input)
		.map(|card| {
			card.contains_winning_numbers()
				.fold(0, |acc, _| if acc != 0 { acc * 2 } else { acc + 1 })
		})
		.sum()
}

fn solve_part02(input: &'static str) -> usize
{
	parse_line::<Card>(input)
		.fold(HashMap::new(), |mut ids, card| {
			let card_id_mut: &mut usize = ids.entry(card.id).or_default();
			card_id_mut.add_assign(1);
			let card_id_cp = *card_id_mut;
			let winning_numbers = card.contains_winning_numbers().count();
			for card_id in card.id.add(1)..=(card.id.add(winning_numbers)) {
				ids.entry(card_id).or_default().add_assign(card_id_cp);
			}
			ids
		})
		.values()
		.sum()
}

fn main()
{
	println!("--- Day 3: Gear Ratios ---");

	let part01 = solve_part01(PUZZLE);
	println!("--- Part One ---");
	println!("\tYour puzzle answer is {}.", solved_output(part01));

	let part02 = solve_part02(PUZZLE);
	println!("--- Part Two ---");
	println!("\tYour puzzle answer is {}.", solved_output(part02));
}

// ---- //
// Test //
// ---- //

#[cfg(test)]
mod tests
{
	use super::*;

	const PUZZLE: &str = "
Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11
";

	#[test]
	fn example_04_01()
	{
		let part01 = solve_part01(PUZZLE.trim());
		assert_eq!(part01, 13);
	}

	#[test]
	fn example_04_02()
	{
		let part02 = solve_part02(PUZZLE.trim());
		assert_eq!(part02, 30);
	}
}
