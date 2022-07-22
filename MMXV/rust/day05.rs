use mmxv_rust::{filter_line, solved_output};

const PUZZLE: &str = include_str!("../puzzles/day05.txt");

fn main() {
	println!("--- Day 5: Doesn't He Have Intern-Elves For This? ---");

	let part01 = filter_line(PUZZLE, solve_part_01).count();
	println!("--- Part One ---");
	println!("\tYour puzzle answer is {}.", solved_output(part01));

	let part02 = filter_line(PUZZLE, solve_part_02).count();
	println!("--- Part Two ---");
	println!("\tYour puzzle answer is {}.", solved_output(part02));
}

fn solve_part_01(line: &str) -> bool {
	contains_vowels(line, 3)
		&& contains_at_least_letter(line, 2)
		&& does_not_contains_chars(line, 2)
}

fn contains_vowels(line: &str, nb: u8) -> bool {
	line.chars()
		.map(|ch| match ch {
			| 'a' | 'e' | 'i' | 'o' | 'u' => 1,
			| _ => 0,
		})
		.sum::<u8>()
		.ge(&nb)
}

fn contains_at_least_letter(line: &str, times: u8) -> bool {
	line.chars()
		.collect::<Vec<_>>()
		.windows(times as usize)
		.any(|arr| arr.first() == arr.get(1))
}

fn does_not_contains_chars(line: &str, size: u8) -> bool {
	line.chars()
		.collect::<Vec<_>>()
		.windows(size as usize)
		.all(|arr| {
			!matches!((arr.first(), arr.get(1)), |(Some('a'), Some('b'))| (
				Some('c'),
				Some('d')
			) | (
				Some('p'),
				Some('q')
			) | (
				Some('x'),
				Some('y')
			))
		})
}

fn solve_part_02(line: &str) -> bool {
	contains_pair_letters(line, 2) && contains_repeated_letter(line, 3)
}

fn contains_pair_letters(line: &str, nb: u8) -> bool {
	let chars = line.chars().collect::<Vec<_>>();

	let mut windows = chars.windows(nb as usize);

	while let Some(window) = windows.next() {
		let following = windows.clone().skip(1).find(|w2| w2 == &window);
		if following.is_some() {
			return true;
		}
	}

	false
}

fn contains_repeated_letter(line: &str, nb: u8) -> bool {
	line.chars()
		.collect::<Vec<char>>()
		.windows(nb as usize)
		.any(|arr| arr.first() == arr.get(2))
}

#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn test_example_01_01() {
		assert!(solve_part_01("ugknbfddgicrmopn"));
	}

	#[test]
	fn test_example_01_02() {
		assert!(solve_part_01("aaa"));
	}

	#[test]
	fn test_example_01_03() {
		assert!(!solve_part_01("jchzalrnumimnmhp"));
	}

	#[test]
	fn test_example_01_04() {
		assert!(!solve_part_01("haegwjzuvuyypxyu"));
	}

	#[test]
	fn test_example_01_05() {
		assert!(!solve_part_01("dvszwmarrgswjxmb"));
	}

	#[test]
	fn test_example_02_01() {
		assert!(solve_part_02("qjhvhtzxzqqjkmpb"));
	}

	#[test]
	fn test_example_02_02() {
		assert!(solve_part_02("xxyxx"));
	}

	#[test]
	fn test_example_02_03() {
		assert!(!solve_part_02("uurcxstgmygtbstg"));
	}

	#[test]
	fn test_example_02_04() {
		assert!(!solve_part_02("ieodomkazucvgmuy"));
	}
}
