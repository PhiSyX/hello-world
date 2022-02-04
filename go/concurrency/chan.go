package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

func get_config(device string) string {
	time.Sleep(time.Duration(rand.Intn(1)) * time.Second)
	return fmt.Sprintf("Connecté à l'appareil %q", device)
}

func connect_to(devices []string, my_channel chan string) {
	var wg sync.WaitGroup

	wg.Add(len(devices))

	for _, device := range devices {
		go func(device string) {
			defer wg.Done()
			my_channel <- get_config(device)
		}(device)
	}

	wg.Wait()

	close(my_channel)
}

func main() {
	devices := []string{"board-1", "board-2", "arduino-ATmega2560"}
	my_channel := make(chan string, len(devices))

	go connect_to(devices, my_channel)

	println("Poursuite de l'exécution...")

	for msg := range my_channel {
		println(msg)
	}
}
