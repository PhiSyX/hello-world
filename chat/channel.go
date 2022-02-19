package chat

import (
	"context"
	"encoding/json"

	"github.com/libp2p/go-libp2p-core/peer"
	pubsub "github.com/libp2p/go-libp2p-pubsub"
)

// --------- //
// Structure //
// --------- //

type Channel struct {
	Message chan *Message
	Event   chan *pubsub.PeerEvent

	Context *context.Context

	Topic        *pubsub.Topic
	Subscription *pubsub.Subscription
}

// -------------- //
// Implémentation //
// -------------- //

func NewChannel(ctx context.Context, ps *pubsub.PubSub, channel_name *string, nick_id peer.ID) (*Channel, error) {
	topic, err := ps.Join(GetTopicName(channel_name))
	if err != nil {
		return nil, err
	}

	subscription, err := topic.Subscribe()
	if err != nil {
		return nil, err
	}

	channel := &Channel{
		Message: make(chan *Message, 128),

		Context: &ctx,

		Topic:        topic,
		Subscription: subscription,
	}

	go channel.read_messages_forever(&nick_id)

	return channel, nil
}

func (channel *Channel) Send(ctx context.Context, message *Message) error {
	bytes, err := json.Marshal(message)
	if err != nil {
		return err
	}

	err = channel.Topic.Publish(ctx, bytes)
	if err != nil {
		return err
	}
	return nil
}

func (channel *Channel) read_messages_forever(nick_id *peer.ID) {
	for {
		message, err := channel.Subscription.Next(*channel.Context)
		if err != nil {
			return
		}

		new_message := new(Message)
		err = json.Unmarshal(message.Data, new_message)
		if err != nil {
			continue
		}

		channel.Message <- new_message
	}
}

// -------- //
// Fonction //
// -------- //

func GetTopicName(channel_name *string) string {
	return *channel_name + " (<name> network)"
}
