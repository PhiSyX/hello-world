use core::fmt;
use std::str::FromStr;

use mmxxiii_rust::{filter_map_line, solved_output};

const PUZZLE: &str = include_str!("../puzzles/day02.txt");

const COLOR_RED: &str = "red";
const COLOR_GREEN: &str = "green";
const COLOR_BLUE: &str = "blue";

const CAPABILITIES: [CubeColor; 3] = [CubeColor::red(12), CubeColor::green(13), CubeColor::blue(14)];

struct CubeColor
{
	cubes: usize,
	color: Color,
}

enum Color
{
	Red,
	Green,
	Blue,
}

impl fmt::Display for Color
{
	fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result
	{
		let color_name = match self {
			| Color::Red => COLOR_RED,
			| Color::Green => COLOR_GREEN,
			| Color::Blue => COLOR_BLUE,
		};
		write!(f, "{}", color_name)
	}
}

impl CubeColor
{
	const fn red(cubes: usize) -> Self
	{
		CubeColor {
			cubes,
			color: Color::Red,
		}
	}

	const fn green(cubes: usize) -> Self
	{
		CubeColor {
			cubes,
			color: Color::Green,
		}
	}

	const fn blue(cubes: usize) -> Self
	{
		CubeColor {
			cubes,
			color: Color::Blue,
		}
	}

	fn color_name(&self) -> String
	{
		self.color.to_string()
	}

	fn update(&mut self, p: usize)
	{
		self.cubes = p.max(self.cubes)
	}
}

impl FromStr for CubeColor
{
	type Err = &'static str;

	fn from_str(mut s: &str) -> Result<Self, Self::Err>
	{
		if s.starts_with(' ') {
			s = s.trim_start();
		}

		let (raw_total_cubes, color) = s.split_once(' ').ok_or("Pourquoi?!")?;

		let total_cubes = raw_total_cubes
			.parse::<usize>()
			.map_err(|_| "N'est pas un nombre usize")?;

		Ok(Self {
			color: color.parse()?,
			cubes: total_cubes,
		})
	}
}

impl FromStr for Color
{
	type Err = &'static str;

	fn from_str(s: &str) -> Result<Self, Self::Err>
	{
		Ok(match s {
			| COLOR_RED => Color::Red,
			| COLOR_GREEN => Color::Green,
			| COLOR_BLUE => Color::Blue,
			| _ => return Err("Seules les couleurs `red`, `green`, `blue` sont attendues"),
		})
	}
}

fn solve_part01(input: &'static str) -> usize
{
	filter_map_line(input, |line| {
		let (game_id, mut cubes_rgb) = parse_game(line)?;

		cubes_rgb
			.all(|cube_rgb| {
				CAPABILITIES
					.into_iter()
					.find(|cap| cap.color_name() == cube_rgb.color_name())
					.filter(|found| cube_rgb.cubes <= found.cubes)
					.is_some()
			})
			.then_some(game_id)
	})
	.sum::<usize>()
}

fn parse_game<'a>(line: &'a str) -> Option<(usize, impl Iterator<Item = CubeColor> + 'a)>
{
	let (raw_game, raw_rgb_cubes) = line.split_once(':')?;

	let game_id: usize = raw_game.trim_start_matches("Game ").parse().ok()?;

	Some((
		game_id,
		raw_rgb_cubes
			.split(';')
			.flat_map(|cubes_rgb| cubes_rgb.split(", "))
			.filter_map(|cube_rgb| cube_rgb.parse::<CubeColor>().ok()),
	))
}

fn solve_part02(input: &'static str) -> usize
{
	filter_map_line(input, |line| {
		let (_, cubes_rgb) = parse_game(line)?;

		let mut fewers: [CubeColor; 3] = [CubeColor::red(0), CubeColor::green(0), CubeColor::blue(0)];

		for cube_rgb in cubes_rgb {
			let fewer = match cube_rgb.color {
				| Color::Red => &mut fewers[0],
				| Color::Green => &mut fewers[1],
				| Color::Blue => &mut fewers[2],
			};

			fewer.update(cube_rgb.cubes);
		}

		Some(fewers[0].cubes * fewers[1].cubes * fewers[2].cubes)
	})
	.sum::<usize>()
}

fn main()
{
	println!("--- Day 2: Cube Conundrum ---");

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

	const PUZZLE: &'static str = "
Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
";

	#[test]
	fn test_example_02_01()
	{
		let part01 = solve_part01(PUZZLE.trim());
		assert_eq!(part01, 8);
	}

	#[test]
	fn test_example_02_02()
	{
		let part02 = solve_part02(PUZZLE.trim());
		assert_eq!(part02, 2286);
	}
}
