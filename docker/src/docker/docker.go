package main

import (
	"fmt"
	"log"
	"net/http"
)

func DockerServer(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Requête: %s", r.URL.Path)
	log.Printf("Reçu pour le chemin: %s", r.URL.Path)
}

func main() {
	var addr string = ":8080"
	handler := http.HandlerFunc(DockerServer)
	if err := http.ListenAndServe(addr, handler); err != nil {
		log.Fatalf("Impossible d'écoute sur le port %s %v", addr, err)
	}
}
