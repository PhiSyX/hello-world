package main

import (
	"context"
	"log"
	"os"
	"os/signal"
	"syscall"
	"time"
)

var (
	listenAddr     = "0.0.0.0"
	listenPort     = 32767
	probeSizeBytes = 9
	maxReadBuffer  = 425984
	retryTimeout   = time.Second * 5
	probeInterval  = time.Second
)

func setup_signal_handlers(cancel context.CancelFunc) {
	signals := make(chan os.Signal, 1)

	signal.Notify(signals, syscall.SIGTERM, syscall.SIGINT, syscall.SIGHUP)

	go func() {
		sig := <-signals
		log.Printf("syscall: %+v", sig)
		cancel()
	}()
}

type probe struct {
	SeqNum uint8
	SendTS int64
}
