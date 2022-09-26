// deno-lint-ignore-file prefer-const
/// <reference path="./shared/types.d.ts" />

import {
	include_str,
	lines,
	max,
	split_by_whitespace,
	vec
} from "./shared/index.ts";

const PUZZLE: string = await include_str("../puzzles/day14.txt");

class Reindeer {
	constructor(
		public name: string,
		private speed: number,
		private duration: number,
		private rest: number,
	) { }

	public distance_for(time: number): number {
		let complete_rush_time = this.duration + this.rest;
		let complete_rush_dist = this.duration * this.speed;

		let repeat = Math.floor(time / complete_rush_time);

		let most_dist = Math.floor(repeat * complete_rush_dist);
		let time_left = Math.floor(time - complete_rush_time * repeat);
		let rest_dist = Math.floor(Math.min(this.duration, time_left) * this.speed);

		return Math.round(most_dist + rest_dist);
	}
}

function main() {
	console.log("--- Day 14: Reindeer Olympics ---");

	console.log("--- Part One ---");
	let part01 = solve_part01(PUZZLE);
	console.log("\tYour puzzle answer is ", part01);
}

function solve_part01(input: string): number {
	return max(
		vec(lines(input))
			.map((line) => {
				let words = vec(split_by_whitespace(line));
				let name = words[0];
				let speed = parseInt(words[3], 10);
				let duration = parseInt(words[6], 10);
				let rest = parseInt(words[13], 10);
				return new Reindeer(name, speed, duration, rest);
			})
			.map((reindeer) => reindeer.distance_for(2503))
	);
}

main();
