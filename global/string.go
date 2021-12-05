package main

import (
	"fmt"
	"unsafe"
)

func literals() {
	str_lit := "str\nliteral"
	str_raw := `str
raw
literal`

	fmt.Println(str_lit)
	fmt.Println(str_raw)

	fmt.Printf("\n" + str_raw + str_lit + "\n\n")
}

func memory() {
	name := "PhiSyX"

	fmt.Println(len(name))

	slice := name[1:5]

	fmt.Println(slice)
	fmt.Printf("name=%T; size=%d bytes\n", name, unsafe.Sizeof(name))
	fmt.Printf("slice=%T; size=%d bytes\n", slice, unsafe.Sizeof(slice))

	// Panic?
	slice = name[4:10]
	fmt.Println(slice) // yes
}

func main() {
	literals()
	memory()
}
