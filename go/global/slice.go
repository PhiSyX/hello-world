package main

import "fmt"

func main() {
	empty_arr_str := []string{}
	arr_str := []string{"zéro", "un", "deux", "trois", "quatre", "cinq", "six"}
	make_arr_str := make([]string, 3)

	fmt.Printf("length=%d; capacity=%d; empty_arr_str=%v;\n", len(empty_arr_str), cap(empty_arr_str), empty_arr_str)
	fmt.Printf("length=%d; capacity=%d; arr_str=%v;\n", len(arr_str), cap(arr_str), arr_str)
	fmt.Printf("length=%d; capacity=%d; make_arr_str=%v;\n", len(make_arr_str), cap(make_arr_str), make_arr_str)

	my_slice := arr_str[1:3]
	fmt.Printf("length=%d; capacity=%d; my_slice=%v;\n", len(my_slice), cap(my_slice), my_slice)

	my_slice = append(my_slice, "sept")
	fmt.Printf("length=%d; capacity=%d; my_slice=%v;\n", len(my_slice), cap(my_slice), my_slice)

	my_slice = append(my_slice, "huit", "neuf", "dix", "onze")
	fmt.Printf("length=%d; capacity=%d; my_slice=%v;\n", len(my_slice), cap(my_slice), my_slice)

	nx1 := []string{"n1", "n2"}
	mutate1(nx1)
	fmt.Printf("nx1=%v;\n", nx1)

	nx2 := []string{"n1", "n2"}
	mutate2(&nx2)
	fmt.Printf("nx2=%v;\n", nx2)
}

func mutate1(input []string) {
	input[0] = "n3"
	input = append(input, "n4")
}

func mutate2(input *[]string) {
	(*input)[0] = "n3"
	*input = append(*input, "n4")
}
