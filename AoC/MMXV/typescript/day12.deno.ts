// deno-lint-ignore-file prefer-const

import { assertEquals } from "https://deno.land/std@0.157.0/testing/asserts.ts";

type PuzzleJson = Array<
	| string
	| number
	| Array<PuzzleJson>
	| { [p: string]: PuzzleJson[number] | Array<PuzzleJson[number]> }
>;

const PUZZLE: PuzzleJson = await include_json("../puzzles/day12.txt");

async function include_json<T>(filename: string): Promise<T> {
	let buffer = await Deno.readFile(filename);
	let decoder = new TextDecoder("UTF-8");
	return JSON.parse(decoder.decode(buffer));
}

const reducer = (c: boolean) => (arr: PuzzleJson): number => {
	return arr
		.filter((i) => !(c && contains_red(i)))
		.map(handle_json(c))
		.reduce((a, b) => a + b, 0);
}

function contains_red(item: PuzzleJson[number]): boolean {
	return is_object(item) && Object.values(item).includes("red");
}

const handle_json = (c: boolean) => (json: PuzzleJson[number]): number => {
	if (is_number(json))
		return json;
	if (is_array(json))
		return reducer(c)(json as PuzzleJson); // <- mauvais typage
	if (is_object(json))
		return reducer(c)(Object.values(json));
	return 0;
}

function is_array<T extends PuzzleJson[number]>(arr: T): arr is T {
	return Array.isArray(arr);
}

function is_object<O extends { constructor: unknown }>(obj: O): obj is O {
	return typeof obj == "object" && obj.constructor == Object;
}

function is_number(n: unknown): n is number {
	return typeof n == "number";
}

function main() {
	console.log("--- Day 12: JSAbacusFramework.io ---");

	console.log("--- Part One ---");
	let part01 = solve_part01(PUZZLE);
	console.log("\tYour puzzle answer is ", part01);

	console.log("--- Part Two ---");
	let part02 = solve_part02(PUZZLE);
	console.log("\tYour puzzle answer is ", part02);
}

function solve_part01(input: PuzzleJson): number {
	return reducer(false)(input);
}

function solve_part02(input: PuzzleJson): number {
	return reducer(true)(input);
}

main();

// ---- //
// Test //
// ---- //

Deno.test("test part01", () => {
	assertEquals(solve_part01([1, 2, 3]), 6);
	assertEquals(solve_part01([{ "a": 2, "b": 4 }]), 6);
	assertEquals(solve_part01([[[3]]]), 3);
	assertEquals(solve_part01([{ "a": { "b": 4 }, "c": -1 }]), 3);
	assertEquals(solve_part01([{ "a": [-1, 1] }]), 0);
	assertEquals(solve_part01([-1, { "a": 1 }]), 0);
	assertEquals(solve_part01([]), 0);
	assertEquals(solve_part01([{}]), 0);
});


Deno.test("test_part02", () => {
	assertEquals(solve_part02([1, 2, 3]), 6);
	assertEquals(solve_part02([1, { "c": "red", "b": 2 }, 3]), 4);
	assertEquals(solve_part02([{ "d": "red", "e": [1, 2, 3, 4], "f": 5 }]), 0);
	assertEquals(solve_part02([1, "red", 5]), 6);
});
