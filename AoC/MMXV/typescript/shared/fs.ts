// deno-lint-ignore-file prefer-const

export
async function include_str<T>(filename: string): Promise<string> {
	let buffer = await Deno.readFile(filename);
	let decoder = new TextDecoder("UTF-8");
	return decoder.decode(buffer);
}

export
function include_json<T>(filename: string): Promise<T> {
	return include_str(filename).then(c => JSON.parse(c));
}
