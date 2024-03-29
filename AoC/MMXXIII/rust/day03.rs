use std::collections::{HashMap, HashSet};

use mmxxiii_rust::{charsln, solved_output};

const PUZZLE: &str = include_str!("../puzzles/day03.txt");

fn solve_part01(input: &'static str) -> usize
{
	let mut gears = Vec::new();

	parse(input, |_, symbols, number, (start, end), (ys, ya)| {
		let mut is_number = false;

		for nx in start..=end {
			for ny in ys..=ya {
				is_number |= symbols.contains(&(nx, ny));
			}
		}

		if is_number {
			gears.push(number);
		}
	});

	gears.iter().sum()
}

fn solve_part02(input: &'static str) -> usize
{
	let mut ratios = HashMap::new();

	parse(input, |chars, symbols, number, (start, end), (ys, ya)| {
		for nx in start..=end {
			for ny in ys..=ya {
				if symbols.contains(&(nx, ny)) && chars[ny][nx] == '*' {
					ratios.entry((nx, ny)).or_insert(vec![]).push(number);
				}
			}
		}
	});

	ratios
		.values()
		.filter_map(|values| (values.len() == 2).then(|| values[0] * values[1]))
		.sum()
}

fn parse(
	input: &'static str,
	mut callback: impl FnMut(&[Vec<char>], &mut HashSet<(usize, usize)>, usize, (usize, usize), (usize, usize)),
)
{
	let chars = charsln(input);

	let mut symbols = HashSet::new();

	for (y, line) in chars.iter().enumerate() {
		for x in line
			.iter()
			.enumerate()
			.filter_map(|(idx, ch)| (!matches!(ch, '0'..='9' | '.')).then_some(idx))
		{
			symbols.insert((x, y));
		}
	}

	for (y, line) in chars.iter().enumerate() {
		let mut gear_ratios = |x, position| {
			let Some(p) = position else {
				return None;
			};
			let [s, e] = [p, x - 1];
			let v = String::from_iter(&line[s..=e]).parse().ok()?;
			callback(
				&chars,
				&mut symbols,
				v,
				(s.saturating_sub(1), e.saturating_add(1)),
				(y.saturating_sub(1), y.saturating_add(1)),
			);
			Some(0)
		};

		let line_len = line.len();

		let position = line.iter().enumerate().fold(None, |old, (idx, ch)| {
			if ch.is_numeric() {
				if old.is_none() {
					return Some(idx);
				}
			} else {
				gear_ratios(idx, old);
				return None;
			}
			old
		});

		gear_ratios(line_len, position);
	}
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
467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..
";

	#[test]
	fn example_03_01()
	{
		let part01 = solve_part01(PUZZLE.trim());
		assert_eq!(part01, 4361);
	}

	#[test]
	fn example_03_02()
	{
		let part02 = solve_part02(PUZZLE.trim());
		assert_eq!(part02, 467835);
	}
}
