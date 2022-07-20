use std::{
	collections::HashSet,
	ops::{Add, AddAssign},
};

use mmxv_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day03.txt");

fn main() {
	println!("--- Day 3: Perfectly Spherical Houses in a Vacuum ---");

	let part01 = solve_part_01(PUZZLE);
	println!("--- Part One ---");
	println!("\tYour puzzle answer is {}.", solved_output(part01));
}

fn parse_direction(input: &'static str) -> impl Iterator<Item = Direction> {
	input.lines().flat_map(|line| {
		line.chars().filter_map(|ch| Direction::try_from(ch).ok())
	})
}

fn solve_part_01(input: &'static str) -> usize {
	let mut coordinate = Coordinate::default();

	let mut visited = HashSet::new();

	visited.insert(coordinate);

	parse_direction(input).for_each(|direction| {
		visited.insert(coordinate.at(direction));
	});

	visited.len()
}

#[derive(Debug)]
#[derive(Default)]
#[derive(Hash, PartialEq, Eq)]
#[derive(Copy, Clone)]
struct Coordinate(i8, i8);

impl Coordinate {
	fn at(&mut self, direction: Direction) -> Self {
		*self += direction;
		*self
	}
}

impl Add<Direction> for Coordinate {
	type Output = Self;

	fn add(self, direction: Direction) -> Self {
		match direction {
			| Direction::North => Self(self.0, self.1 - 1),
			| Direction::East => Self(self.0 - 1, self.1),
			| Direction::South => Self(self.0, self.1 + 1),
			| Direction::West => Self(self.0 + 1, self.1),
		}
	}
}

impl<T> AddAssign<T> for Coordinate
where
	Self: Add<T, Output = Self>,
{
	fn add_assign(&mut self, other: T) {
		*self = *self + other
	}
}

#[derive(Debug)]
#[derive(PartialEq, Eq)]
enum Direction {
	North,
	East,
	South,
	West,
}

impl TryFrom<char> for Direction {
	type Error = &'static str;

	fn try_from(ch: char) -> Result<Self, Self::Error> {
		Ok(match ch {
			| '^' => Self::North,
			| 'v' => Self::South,
			| '>' => Self::East,
			| '<' => Self::West,
			| _ => return Err("Direction inconnue"),
		})
	}
}

#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn test_example_01_01() {
		let puzzle_input: &str = ">";
		assert_eq!(solve_part_01(puzzle_input), 2);
	}

	#[test]
	fn test_example_01_02() {
		let puzzle_input: &str = "^>v<";
		assert_eq!(solve_part_01(puzzle_input), 4);
	}

	#[test]
	fn test_example_01_03() {
		let puzzle_input: &str = "^v^v^v^v^v";
		assert_eq!(solve_part_01(puzzle_input), 2);
	}
}
