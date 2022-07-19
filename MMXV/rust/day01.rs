use mmxv_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day01.txt");

fn main() {
	println!("--- Day 1: Not Quite Lisp ---");

	let part01 = solve_part_01(PUZZLE);
	println!("--- Part One ---");
	println!("\tYour puzzle answer is {}.", solved_output(part01));

	let (_, maybe_part02) = solve_part_02(PUZZLE);
	let part02 = maybe_part02.unwrap_or_default();
	println!("--- Part Two ---");
	println!("\tYour puzzle answer is {}.", solved_output(part02));
}

fn solve_part_01(input: &str) -> isize {
	input.chars().fold(0, |acc, ch| match ch {
		| '(' => acc + 1,
		| ')' => acc - 1,
		| _ => acc,
	})
}

fn solve_part_02(input: &str) -> (isize, Option<usize>) {
	input.chars().enumerate().fold(
		(0, None),
		|(lfloor, basement), (ch_idx, ch)| match ch {
			| '(' => (lfloor + 1, basement),

			| ')' => {
				let rfloor = lfloor - 1;
				match basement {
					| Some(pos) => (rfloor, Some(pos)),
					| None if rfloor == -1 => (rfloor, Some(ch_idx + 1)),
					| None => (rfloor, None),
				}
			}

			| _ => (lfloor, basement),
		},
	)
}

#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn test_example_01_01() {
		let puzzle_input: &str = "(())";
		assert_eq!(solve_part_01(puzzle_input), 0);

		let puzzle_input: &str = "()()";
		assert_eq!(solve_part_01(puzzle_input), 0);
	}

	#[test]
	fn test_example_01_02() {
		let puzzle_input: &str = "(((";
		assert_eq!(solve_part_01(puzzle_input), 3);

		let puzzle_input: &str = "(()(()(";
		assert_eq!(solve_part_01(puzzle_input), 3);
	}

	#[test]
	fn test_example_01_03() {
		let puzzle_input: &str = "))(((((";
		assert_eq!(solve_part_01(puzzle_input), 3);
	}

	#[test]
	fn test_example_01_04() {
		let puzzle_input: &str = "())";
		assert_eq!(solve_part_01(puzzle_input), -1);

		let puzzle_input: &str = "))(";
		assert_eq!(solve_part_01(puzzle_input), -1);
	}

	#[test]
	fn test_example_01_05() {
		let puzzle_input: &str = ")))";
		assert_eq!(solve_part_01(puzzle_input), -3);

		let puzzle_input: &str = ")())())";
		assert_eq!(solve_part_01(puzzle_input), -3);
	}

	#[test]
	fn test_example_02_01() {
		let puzzle_input: &str = ")";
		assert!(matches!(solve_part_02(puzzle_input), (_, Some(1))));
	}

	#[test]
	fn test_example_02_02() {
		let puzzle_input: &str = "()())";
		assert!(matches!(solve_part_02(puzzle_input), (_, Some(5))));
	}
}
