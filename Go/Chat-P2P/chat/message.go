package chat

import (
	peer "github.com/libp2p/go-libp2p-core/peer"
)

// --------- //
// Structure //
// --------- //

type Line struct {
	Message string        `json:"message"`
	Sender  MessageSender `json:"sender"`
}

type MessageSender struct {
	ID   peer.ID `json:"peer_id"`
	Nick string  `json:"nick"`
}
