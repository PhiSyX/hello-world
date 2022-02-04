package main

import "fmt"

func main() {
	cond := true
	if cond {
		fmt.Printf("type=%T;value=%t;\n", cond, cond)
	}
}
