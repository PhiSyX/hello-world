use std::{
	ops::{AddAssign, Index, IndexMut},
	str::FromStr,
};

use mmxv_rust::solved_output;

const PUZZLE: &str = include_str!("../puzzles/day06.txt");

fn main() {
	println!("--- Day 2: I Was Told There Would Be No Math ---");

	const GRID_SIZE: usize = 1000;

	let part01 = solve_part_01::<GRID_SIZE>(PUZZLE);
	println!("--- Part One ---");
	println!("\tYour puzzle answer is {}.", solved_output(part01));
}

fn solve_part_01<const N: usize>(input: &str) -> usize {
	let mut grid = Grid::<N>::default();

	parse(input, &mut grid, |p, lt| match lt {
		| Light::TurnOn => *p = 1,
		| Light::TurnOff => *p = 0,
		| Light::Toggle => *p = if *p == 0 { 1 } else { 0 },
	});

	grid.iter().sum()
}

#[derive(Clone, Debug)]
struct Grid<const N: usize>(Vec<Vec<usize>>);

type PositionX = usize;
type PositionY = usize;

#[derive(Copy, Clone, Debug)]
struct Position(PositionX, PositionY);

struct GridPosition {
	start: Position,
	end: Position,
	current: Position,
	done: bool,
}

#[derive(Copy, Clone, Debug)]
enum Light {
	TurnOn,
	TurnOff,
	Toggle,
}

impl<const N: usize> Grid<N> {
	fn iter(&self) -> impl Iterator<Item = &usize> {
		self.0.iter().flat_map(|sub| sub.iter())
	}
}

impl<const N: usize> Grid<N> {
	fn positions(
		&mut self,
		base: Position,
		end: Position,
	) -> impl Iterator<Item = &mut usize> {
		GridPosition::from(base, end)
			.map(|position| unsafe { std::mem::transmute(&mut self[position]) })
	}
}

impl GridPosition {
	fn from(start: Position, end: Position) -> Self {
		Self {
			start,
			end,
			current: start,
			done: false,
		}
	}
}

impl<const N: usize> Default for Grid<N> {
	fn default() -> Self {
		Self(vec![vec![usize::default(); N]; N])
	}
}

impl<const N: usize> Index<Position> for Grid<N> {
	type Output = usize;

	fn index(&self, index: Position) -> &Self::Output {
		&self.0[index.1][index.0]
	}
}

impl<const N: usize> IndexMut<Position> for Grid<N> {
	fn index_mut(&mut self, index: Position) -> &mut Self::Output {
		&mut self.0[index.1][index.0]
	}
}

impl FromStr for Position {
	type Err = &'static str;

	fn from_str(input: &str) -> Result<Self, Self::Err> {
		let positions: Vec<&str> = input.split(',').collect();

		let (x, y): (usize, usize) = (
			positions
				.first()
				.and_then(|x| x.parse().ok())
				.expect("Le premier élément"),
			positions
				.get(1)
				.and_then(|y| y.parse().ok())
				.expect("Le second élément"),
		);

		Ok(Self(x, y))
	}
}

impl FromStr for Light {
	type Err = &'static str;

	fn from_str(w: &str) -> Result<Self, Self::Err> {
		Ok(match w {
			| "turn on" => Self::TurnOn,
			| "turn off" => Self::TurnOff,
			| "toggle" => Self::Toggle,
			| _ => return Err("Terme inconnu"),
		})
	}
}

impl Iterator for GridPosition {
	type Item = Position;

	fn next(&mut self) -> Option<Self::Item> {
		let current = self.current;

		if self.done {
			return None;
		}

		if self.current.0 == self.end.0 && self.current.1 == self.end.1 {
			self.done = true;
		} else if self.current.0 == self.end.0 {
			self.current.0 = self.start.0;
			self.current.1.add_assign(1);
		} else {
			self.current.0.add_assign(1);
		}

		Some(current)
	}
}

// -------- //
// Fonction //
// -------- //

fn parse<const N: usize>(
	input: &str,
	grid: &mut Grid<N>,
	part_fn: fn(&mut usize, Light),
) {
	input.lines().for_each(|line| {
		let mut lights: Vec<_> = line.rsplitn(4, ' ').collect();

		lights.reverse();

		let lt = lights.first().and_then(|t| t.parse().ok()).unwrap();
		let base = lights.get(1).and_then(|b| b.parse().ok()).unwrap();
		let end = lights.get(3).and_then(|e| e.parse().ok()).unwrap();

		grid.positions(base, end).for_each(|pos| part_fn(pos, lt));
	});
}

#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn test_example_01_01() {
		let line = "turn on 0,0 through 999,999";
		assert_eq!(solve_part_01::<1000>(line), 1_000_000);
	}

	#[test]
	fn test_example_01_02() {
		let line = "toggle 0,0 through 999,0";
		assert_eq!(solve_part_01::<1000>(line), 1_000_000);
	}

	#[test]
	fn test_example_01_03() {
		let line = "turn off 499,499 through 500,500";
		assert_eq!(solve_part_01::<1000>(line), 0);
	}
}
