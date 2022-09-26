// deno-lint-ignore-file prefer-const
/// <reference path="./types.d.ts" />

export
function array_windows(iter: Iter<string>, size: number): Array<Array<string>> {
	let result: Array<Array<string>> = [];
	vec(iter).some((_, index, arr) => {
		if (index + size > arr.length)
			return true;
		result.push(arr.slice(index, index + size));
	});
	return result;
}

export
function dbg<T>(args: T): T {
	console.log(args);
	return args;
}

export
function permutations<T>(iter: Iter<T>, size?: number): Array<Array<T>> {
	let pool = [...iter];
	let len = size ?? pool.length;
	let result = [pool.slice()];
	let c = new Array(len).fill(0);
	let i = 1;
	while (i < len) {
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

export
function vec<T>(iter: Iter<T>): Array<T>
{
	return [...iter];
}

export
function max(iter: Iter<number>): number {
	return [...iter].reduce((max, v) => max >= v ? max : v, -Infinity);
}

export
function sum(iter: Iter<number>): number {
	return [...iter].reduce((a, b) => a + b, 0);
}