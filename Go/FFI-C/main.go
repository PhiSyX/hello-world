package main

// #cgo CFLAGS: -g -Wall -I${SRCDIR}/test
// #cgo LDFLAGS: -L${SRCDIR}/test -lffi
// #include "add.c"
// #include "sub.c"

import "C"

func main() {
	println("Expression arithmétique: 10 + 32 = ", C.add(10, 32))
	println("Expression arithmétique: 80 - 77 = ", C.sub(80, 77))
}
