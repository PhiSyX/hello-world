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

function sum(iter: Iter<number>): number {
	return [...iter].reduce((a, b) => a + b, 0);
}

function main() {
	console.log("--- Day 13: Knights of the Dinner Table ---");

	console.log("--- Part One ---");
	let part01 = solve_part01(PUZZLE);
	console.log("\tYour puzzle answer is ", part01);
}

function solve_part01(input: string): number {
	let map = new Map<string, number>();
	let peoples = new Set<string>();

	for (const line of lines(input)) {
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
		map.set(`${l}@${r}`, n);
		peoples.add(l);
	}

	const mget = (arr: Array<string>, a: number, b: number) =>
		map.get(`${arr[a]}@${arr[b]}`)!;

	let happiness = Math.max(
		...permutations(peoples, peoples.size).map((relations) => {
			return sum(
					array_windows(relations, 2)
						.map((arr) => mget(arr, 0, 1) + mget(arr, 1, 0))
				)
				+ map.get(`${relations.at(-1)!}@${relations[0]}`)!
				+ map.get(`${relations[0]}@${relations.at(-1)!}`)!;
		})
	);

	return happiness;
}

main();
