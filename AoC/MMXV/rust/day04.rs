// crate - https://crates.io/crates/md5

use mmxv_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day04.txt");

fn main() {
	println!("--- Day 4: The Ideal Stocking Stuffer ---");

	let part01 = solve_part_01(PUZZLE.trim());
	println!("--- Part One ---");
	println!("\tYour puzzle answer is {}.", solved_output(part01));

    let part02 = solve_part_02(PUZZLE.trim());
    println!("--- Part Two ---");
    println!("\tYour puzzle answer is {}.", solved_output(part02));
}

fn solve<const N: usize>(input: &str) -> usize {
	let mut id = 0;

	loop {
		let base = format!("{input}{id}");
		let digest = md5::compute(base);

		if digest[0..N] == [0; N] && digest[2] < 0xF {
			break id;
		}

		id += 1;
	}
}

fn solve_part_01(input: &str) -> usize {
	solve::<2>(input)
}

fn solve_part_02(input: &str) -> usize {
	solve::<3>(input)
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
