package main

import "fmt"

type MyStruct struct {
	name string
}

func (d *MyStruct) SetName() {
	d.name = "John, " + d.name
}

func (d MyStruct) GetName() string {
	return d.name
}

func main() {
	ms1 := MyStruct{name: "John, Doe"}
	fmt.Println(ms1.GetName())

	ms2 := MyStruct{name: "Doe"}
	ms2.SetName()
	fmt.Println(ms2.GetName())
}
