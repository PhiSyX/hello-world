package main

import (
	"fmt"
)

func main() {
	hostnames := [2]string{"localhost", "local.web"}

	ips := [2]string{
		"127.0.0.1/8",
		"::1/128",
	}

	fmt.Println(hostnames[1])
	fmt.Println(ips[0])

	var ip_addr [4]byte
	var localhost = [4]byte{127, 0, 0, 1}

	fmt.Printf("%b\n", ip_addr)

	fmt.Printf("%b\n", localhost)
	fmt.Println(len(localhost))

	fmt.Println(ip_addr == localhost)
}
