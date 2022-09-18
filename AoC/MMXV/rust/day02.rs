use std::{ops::Add, str::FromStr};

use mmxv_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day02.txt");

fn main() {
	println!("--- Day 2: I Was Told There Would Be No Math ---");

	let part01 = parse_cuboid(PUZZLE, solve_part_01);
	println!("--- Part One ---");
	println!("\tYour puzzle answer is {}.", solved_output(part01));

	let part02 = parse_cuboid(PUZZLE, solve_part_02);
	println!("--- Part Two ---");
	println!("\tYour puzzle answer is {}.", solved_output(part02));
}

fn parse_cuboid(input: &str, part: fn(&Cuboid) -> isize) -> isize {
	input
		.lines()
		.filter_map(|line| line.parse().ok())
		.map(|ref cuboid| part(cuboid))
		.sum()
}

fn solve_part_01(cuboid: &Cuboid) -> isize {
	cuboid.find_surface_area().add(cuboid.smallest_side())
}

fn solve_part_02(cuboid: &Cuboid) -> isize {
	cuboid.cubic_feet_of_volume_of_present()
}

#[derive(Debug)]
struct Cuboid(isize, isize, isize);

impl Cuboid {
	fn find_surface_area(&self) -> isize {
		let (l, w, h) = (self.0, self.1, self.2);
		2 * (l * w + w * h + h * l)
	}

	fn smallest_side(&self) -> isize {
		let (l, w, h) = (self.0, self.1, self.2);
		[l * w, w * h, l * h].into_iter().min().unwrap_or_default()
	}

	fn cubic_feet_of_volume_of_present(&self) -> isize {
		let (l, w, h) = (self.0, self.1, self.2);
		let volume = l * w * h;
		volume.add(
			[2 * l + 2 * w, 2 * w + 2 * h, 2 * l + 2 * h]
				.into_iter()
				.min()
				.unwrap_or_default(),
		)
	}
}

impl FromStr for Cuboid {
	type Err = ();

	fn from_str(s: &str) -> Result<Self, Self::Err> {
		let mut parts = s.split('x').filter_map(|p| p.parse().ok()).take(3);
		let (one, two, three) = (
			parts.next().unwrap(),
			parts.next().unwrap(),
			parts.next().unwrap(),
		);
		Ok(Self(one, two, three))
	}
}

#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn test_example_01_01() {
		let puzzle_input: &str = "2x3x4";
		assert_eq!(parse_cuboid(puzzle_input, solve_part_01), 58);
	}

	#[test]
	fn test_example_01_02() {
		let puzzle_input: &str = "1x1x10";
		assert_eq!(parse_cuboid(puzzle_input, solve_part_01), 43);
	}

	#[test]
	fn test_example_02_01() {
		let puzzle_input: &str = "2x3x4";
		assert_eq!(parse_cuboid(puzzle_input, solve_part_02), 34);
	}

	#[test]
	fn test_example_02_02() {
		let puzzle_input: &str = "1x1x10";
		assert_eq!(parse_cuboid(puzzle_input, solve_part_02), 14);
	}
}
