package chat

import (
	pubsub "github.com/libp2p/go-libp2p-pubsub"
)

// --------- //
// Structure //
// --------- //

type Channel struct {
	Message chan *Message
	Topic   *pubsub.Topic
}

// -------------- //
// Implémentation //
// -------------- //

func NewChannel(ps *pubsub.PubSub, channel_name *string) (*Channel, error) {
	topic, err := ps.Join(GetTopicName(channel_name))

	if err != nil {
		return nil, err
	}

	channel := &Channel{
		Message: make(chan *Message, 128),
		Topic:   topic,
	}

	return channel, nil
}

// -------- //
// Fonction //
// -------- //

func GetTopicName(channel_name *string) string {
	return *channel_name + " (<name> network)"
}
