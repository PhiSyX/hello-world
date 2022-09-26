/// <reference path="./types.d.ts" />

export
function lines(str: string): Iter<string> {
	return str.split(/\r|\n/g).filter(Boolean);
}

export
function split_by_whitespace(str: string): Iter<string> {
	return str.split(/\s/g);
}
