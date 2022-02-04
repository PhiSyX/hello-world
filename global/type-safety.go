package main

func process(input string) string {
	return "Hello, " + input
}

func main() {
	output := process("World")
	println(output)

	// ne compile(ra) pas car le type de input n'est pas string
	output42 := process(42)
	println(output42)
}
