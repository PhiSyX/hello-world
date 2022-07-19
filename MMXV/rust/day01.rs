const PUZZLE: &str = include_str!("../puzzles/day01.txt");

fn main() {
    let part01 = solve_part_01(PUZZLE);
    assert_eq!(part01, 280);
    println!("Your puzzle answer is {part01}.");
}

fn solve_part_01(input: &str) -> isize {
	input.chars().fold(0, |acc, ch| match ch {
		| '(' => acc + 1,
		| ')' => acc - 1,
		| _ => acc,
	})
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
}
