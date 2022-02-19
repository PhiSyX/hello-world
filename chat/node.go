package chat

import (
	host "github.com/libp2p/go-libp2p-core/host"
	pubsub "github.com/libp2p/go-libp2p-pubsub"
)

// --------- //
// Structure //
// --------- //

type NodeAPI struct {
	Host   *host.Host
	PubSub *pubsub.PubSub
}
