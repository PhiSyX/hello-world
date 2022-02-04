package main

import (
	"fmt"
	"net/http"
)

func main() {
	resp, err := http.Get("http://google.com/")
	if err != nil {
		panic(err)
	}

	defer resp.Body.Close()
	defer fmt.Println("Différer le nettoyage")

	fmt.Println("Status:", resp.Status)
}
