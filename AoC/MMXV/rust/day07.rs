use core::str::FromStr;
use std::collections::HashMap;

use mmxv_rust::{filter_map_line, solved_output};

const PUZZLE: &str = include_str!("../puzzles/day07.txt");

type Map = HashMap<Bitwise, Memoize>;

#[derive(Clone, Debug)]
enum Memoize {
	Eval(Bitwise),
	Memoized(u16),
}

impl Memoize {
	fn get(&self, cache: &mut Map) -> Self {
		match self {
			| Self::Eval(bit) => Self::Memoized(bit.eval(cache)),
			| done => done.clone(),
		}
	}

	fn into_value(self) -> u16 {
		match self {
			| Self::Memoized(n) => n,
			| Self::Eval(_) => unreachable!(),
		}
	}
}

#[derive(PartialEq, Eq, Hash, Clone, Debug)]
enum Bitwise {
	Not,
	And,
	Or,
	LShift,
	RShift,

	Unknown(String),
}

impl Bitwise {
	fn eval(&self, cache: &mut Map) -> u16 {
		if !matches!(self, Self::Unknown(..)) {
			return 0;
		}

		let mut parts = self.splitw();

		let first_bit: Bitwise =
			parts.next().map(Into::into).expect("le premier bit");

		let second_bit: Option<Bitwise> = parts.next().map(Into::into);
		let third_bit: Option<Bitwise> = parts.next().map(Into::into);

		match (first_bit, second_bit, third_bit) {
			| (Bitwise::Not, Some(s), _) => !s.eval(cache),

			| (f, Some(Bitwise::And), Some(t)) => f.eval(cache) & t.eval(cache),
			| (f, Some(Bitwise::Or), Some(t)) => f.eval(cache) | t.eval(cache),
			| (f, Some(Bitwise::LShift), Some(t)) => {
				f.eval(cache) << t.eval(cache)
			}
			| (f, Some(Bitwise::RShift), Some(t)) => {
				f.eval(cache) >> t.eval(cache)
			}

			| (f, None, None) => f.parse().unwrap_or_else(|_| {
				let mem = unsafe {
					cache
						.get(&f)
						.cloned()
						.map(|m| m.get(cache))
						.unwrap_unchecked()
				};
				cache.insert(f, mem.clone());
				mem.into_value()
			}),

			| _ => unreachable!(),
		}
	}

	fn parse<T>(&self) -> Result<T, T::Err>
	where
		T: FromStr,
	{
		match self {
			| Self::Unknown(s) => s.parse(),
			| _ => unimplemented!(),
		}
	}

	fn splitw(&self) -> impl Iterator<Item = &str> {
		match self {
			| Self::Unknown(s) => s.split_whitespace(),
			| _ => unimplemented!(),
		}
	}
}

impl From<&str> for Bitwise {
	fn from(op: &str) -> Self {
		match op {
			| "NOT" => Self::Not,
			| "AND" => Self::And,
			| "OR" => Self::Or,
			| "LSHIFT" => Self::LShift,
			| "RSHIFT" => Self::RShift,
			| _ => Self::Unknown(op.to_owned()),
		}
	}
}

fn main() {
	println!("--- Day 7: Some Assembly Required ---");

	let part01 = solve_part_01(PUZZLE, "a");
	println!("--- Part One ---");
	println!("\tYour puzzle answer is {}.", solved_output(part01));

	let part02 = solve_part_02(PUZZLE, "a", "b");
	println!("--- Part Two ---");
	println!("\tYour puzzle answer is {}.", solved_output(part02));
}

fn solve_part(input: &'static str, wire: &str) -> Memoize {
	let cache: Map = parse_input(input).collect();
	solve_in_cache(cache, wire)
}

fn solve_in_cache(mut cache: Map, wire: &str) -> Memoize {
	let bit: Bitwise = wire.into();
	unsafe {
		cache
			.get(&bit)
			.cloned()
			.map(|mem| mem.get(&mut cache))
			.unwrap_unchecked()
	}
}

fn solve_part_01(input: &'static str, wire: &str) -> u16 {
	solve_part(input, wire).into_value()
}

fn solve_part_02(input: &'static str, wire1: &str, wire2: &str) -> u16 {
	let w1 = solve_part(input, wire1);
	let mut cache: Map = parse_input(input).collect();

	let bit2: Bitwise = wire2.into();
	if let Some(w2c) = cache.get_mut(&bit2) {
		*w2c = w1;
	}

	solve_in_cache(cache, wire1).into_value()
}

fn parse_input(
	input: &'static str,
) -> impl Iterator<Item = (Bitwise, Memoize)> {
	filter_map_line(input, |line| {
		let mut parts = line.rsplit("->").map(str::trim);

		let first = parts.next().map(Into::into);
		let end = parts.next().map(Into::into);

		match (first, end) {
			| (Some(f), Some(e)) => Some((f, Memoize::Eval(e))),
			| _ => None,
		}
	})
}

// ---- //
// Test //
// ---- //

#[cfg(test)]
mod tests {
	use super::*;

	const CIRCUIT: &str = "123 -> x
456 -> y
x AND y -> d
x OR y -> e
x LSHIFT 2 -> f
y RSHIFT 2 -> g
NOT x -> h
NOT y -> i";

	#[test]
	fn test_part01() {
		let line = solve_part_01(CIRCUIT, "d");
		assert_eq!(line, 72);

		let line = solve_part_01(CIRCUIT, "e");
		assert_eq!(line, 507);

		let line = solve_part_01(CIRCUIT, "f");
		assert_eq!(line, 492);

		let line = solve_part_01(CIRCUIT, "g");
		assert_eq!(line, 114);

		let line = solve_part_01(CIRCUIT, "h");
		assert_eq!(line, 65412);

		let line = solve_part_01(CIRCUIT, "i");
		assert_eq!(line, 65079);

		let line = solve_part_01(CIRCUIT, "x");
		assert_eq!(line, 123);

		let line = solve_part_01(CIRCUIT, "y");
		assert_eq!(line, 456);
	}
}
