// crate - https://crates.io/crates/md5

use mmxv_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day04.txt");

fn main() {
	println!("--- Day 4: The Ideal Stocking Stuffer ---");

	let part01 = solve_part_01(PUZZLE.trim());
	println!("--- Part One ---");
	println!("\tYour puzzle answer is {}.", solved_output(part01));
}

fn solve_part_01(input: &str) -> usize {
	let mut id = 0;

	loop {
		let base = format!("{input}{id}");
		let digest = md5::compute(base);

		if digest[0..2] == [0; 2] && digest[2] < 0xF {
			break id;
		}

		id += 1;
	}
}

#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn test_example_01_01() {
		let puzzle_input = "abcdef";
		assert_eq!(solve_part_01(puzzle_input), 609043);
	}

	#[test]
	fn test_example_01_02() {
		let puzzle_input = "pqrstuv";
		assert_eq!(solve_part_01(puzzle_input), 1048970);
	}
}
