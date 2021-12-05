package main

import (
	"context"
	"encoding/binary"
	"flag"
	"log"
	"net"
	"time"
)

func receive(udpConn net.UDPConn) {
	log.Printf("Réception UDP")

	var nextSeq uint8
	var lost int
	for {
		p := &probe{}
		if err := binary.Read(&udpConn, binary.BigEndian, p); err != nil {
			return
		}

		log.Printf("Reçu %d", p.SeqNum)
		if p.SeqNum < nextSeq {
			log.Printf("packet seq/exp: %d/%d", p.SeqNum, nextSeq)
			lost -= 1
		} else if p.SeqNum > nextSeq {
			log.Printf("packet seq/exp: %d/%d", p.SeqNum, nextSeq)
			lost += int(p.SeqNum - nextSeq)
			nextSeq = p.SeqNum
		}

		latency := time.Now().UnixMilli() - p.SendTS
		log.Printf("latences E2E: %d ms", latency)
		log.Printf("paquets perdues: %d", lost)

		nextSeq++
	}
}

func main() {
	server := flag.String("server", "127.0.0.1", "IP serveur")
	port := flag.Int("port", 32767, "Port serveur")
	flag.Parse()

	udp_addr := &net.UDPAddr{
		IP:   net.ParseIP(*server),
		Port: *port,
	}

	upd_conn, err := net.DialUDP("udp", nil, udp_addr)
	if err != nil {
		log.Fatalf("panic net.DialUDP: %s", err)
	}
	defer upd_conn.Close()

	ctx, cancel := context.WithCancel(context.Background())
	setup_signal_handlers(cancel)

	ticker := time.NewTicker(probeInterval)

	log.Printf("UDP ping client")
	go receive(*upd_conn)

	var seq uint8
	for {
		select {
		case <-ctx.Done():
			log.Printf("Done client")
			return

		case <-ticker.C:
			log.Printf("send seq %d", seq)
			p := &probe{
				SeqNum: seq,
				SendTS: time.Now().UnixMilli(),
			}

			if err := upd_conn.SetWriteDeadline(time.Now().Add(retryTimeout)); err != nil {
				log.Fatalf("panic udp_conn.SetWriteDeadline: %s", err)
			}

			if err := binary.Write(upd_conn, binary.BigEndian, p); err != nil {
				log.Fatalf("panic binary.Write: %v", err)
			}

			seq++
		}
	}

}
