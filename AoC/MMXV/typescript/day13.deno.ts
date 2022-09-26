// deno-lint-ignore-file prefer-const

type Iter<T> = {
	[Symbol.iterator](): Iterator<T>
};

const PUZZLE: string = await include_str("../puzzles/day13.txt");

async function include_str<T>(filename: string): Promise<string> {
	let buffer = await Deno.readFile(filename);
	let decoder = new TextDecoder("UTF-8");
	return decoder.decode(buffer);
}

function parse(line: string): [string, string, number] {
	let words = split_by_whitespace(line.slice(0, -1));
	let [l, r, g, v] = [words[0], words[10], words[2], words[3]];
	let n: number;
	switch (g) {
		case "gain":
			{
				n = parseInt(v, 10);
			} break;

		case "lose":
			{
				n = - parseInt(v, 10);
			} break;

		default:
			throw new Error(`Valeur inattendue -- {g}`);
	}

	return [l, r, n]
}

function lines(str: string): Array<string> {
	return str.split(/\r|\n/g).filter(Boolean);
}

function split_by_whitespace(str: string): Array<string> {
	return str.split(/\s/g);
}

function array_windows(iter: Iter<string>, size: number): Array<Array<string>> {
	let result: Array<Array<string>> = [];
	let arr = [...iter];
	arr.some((_, i: number) => {
		if (i + size > arr.length)
			return true;
		result.push(arr.slice(i, i + size));
	});
	return result;
}

function permutations<T>(iter: Iter<T>, len?: number): Array<Array<T>> {
	let pool = [...iter];
	let size = len ?? pool.length;
	let result = [pool.slice()];
	let c = new Array(size).fill(0);
	let i = 1;
	while (i < size) {
		if (c[i] < i) {
			let k = i % 2 && c[i];
			let p = pool[i];
			pool[i] = pool[k];
			pool[k] = p;
			++c[i];
			i = 1;
			result.push(pool.slice());
		} else {
			c[i] = 0;
			++i;
		}
	}
	return result;
}

function max(iter: Iter<number>): number {
	return [...iter].reduce((max, v) => max >= v ? max : v, -Infinity);
}

function sum(iter: Iter<number>): number {
	return [...iter].reduce((a, b) => a + b, 0);
}

function happiness(map: Map<string, number>, peoples: Set<string>): number {
	const mget = (arr: Array<string>, a: number, b: number) =>
		map.get(`${arr.at(a)}@${arr.at(b)}`)!
	return max(
		permutations(peoples, peoples.size).map((relations) =>
			sum(
				array_windows(relations, 2)
					.map((arr) => mget(arr, 0, 1) + mget(arr, 1, 0))
			)
				+ mget(relations, -1, 0)
				+ mget(relations, 0, -1))
	);
}

function main() {
	console.log("--- Day 13: Knights of the Dinner Table ---");

	console.log("--- Part One ---");
	let part01 = solve_part01(PUZZLE);
	console.log("\tYour puzzle answer is ", part01);

	console.log("--- Part Two ---");
	let part02 = solve_part02(PUZZLE);
	console.log("\tYour puzzle answer is ", part02);
}

function solve_part01(input: string): number {
	let map = new Map<string, number>();
	let peoples = new Set<string>();

	for (const line of lines(input)) {
		let [l, r, n] = parse(line);
		map.set(`${l}@${r}`, n);
		peoples.add(l);
	}

	return happiness(map, peoples);
}

function solve_part02(input: string): number {
	let map = new Map<string, number>();
	let peoples = new Set<string>();
	peoples.add("me");

	for (const line of lines(input)) {
		let [l, r, n] = parse(line);
		map.set(`${l}@${r}`, n);
		map.set(`me@${r}`, 0);
		map.set(`${l}@me`, 0);
		peoples.add(l);
	}
	return happiness(map, peoples);
}

main();
