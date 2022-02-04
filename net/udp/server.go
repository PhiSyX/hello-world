package main

import (
	"context"
	"flag"
	"log"
	"net"
	"time"
)

func main() {
	port := flag.Int("port", listenPort, "port d'écoute")
	flag.Parse()

	listen_addr := &net.UDPAddr{
		IP:   net.ParseIP(listenAddr),
		Port: *port,
	}

	udp_conn, err := net.ListenUDP("udp", listen_addr)
	if err != nil {
		log.Fatalf("panic: %s:%d: %s", listen_addr, *port, err)
	}
	defer udp_conn.Close()

	ctx, cancel := context.WithCancel(context.Background())
	setup_signal_handlers(cancel)

	if err = udp_conn.SetReadBuffer(maxReadBuffer); err != nil {
		log.Fatalf("panic udp_conn.SetReadBuffer: %s", err)
	}

	log.Printf("Ping server")
	for {
		select {
		case <-ctx.Done():
			log.Printf("Done server")
			return
		default:
			bytes := make([]byte, maxReadBuffer)

			if err := udp_conn.SetReadDeadline(time.Now().Add(retryTimeout)); err != nil {
				log.Fatalf("panic udp_conn.SetReadDeadline: %s", err)
			}

			len, raddr, err := udp_conn.ReadFromUDP(bytes)
			if err != nil {
				log.Printf("panic udp_conn.ReadFromUDP: %s", err)
				continue
			}
			log.Printf("reçu: %s:%d", raddr.IP.String(), raddr.Port)

			if len == 0 {
				continue
			}

			n, err := udp_conn.WriteToUDP(bytes[:len], raddr)
			if err != nil {
				log.Fatalf("panic: udp_conn.WriteToUDP: %s", err)
			}

			if n != len {
				log.Printf("n'a pas pu envoyer le paquet complet")
			}
		}
	}

}
