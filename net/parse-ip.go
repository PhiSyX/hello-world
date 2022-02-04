package main

import (
	"fmt"
	"log"
	"net"
)

func main() {
	ipv4 := net.ParseIP("192.0.2.1")
	fmt.Println(ipv4)

	ipv6 := net.ParseIP("FC02:F00D::1")
	fmt.Println(ipv6)

	fmt.Println(ipv4.IsPrivate())
	fmt.Println(ipv6.IsPrivate())

	fmt.Printf("%b\n", net.CIDRMask(31, 32))

	fmt.Printf("%s\n", net.CIDRMask(64, 128))

	ipv4Addr, ipv4Net, err := net.ParseCIDR("192.0.2.1/24")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(ipv4Addr)
	fmt.Println(ipv4Net)

	ipv6Addr, ipv6Net, err := net.ParseCIDR("2001:db8:a0b:12f0::1/32")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(ipv6Addr)
	fmt.Println(ipv6Net)
}
