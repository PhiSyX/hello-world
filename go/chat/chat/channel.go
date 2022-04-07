package chat

import (
	"context"
	"encoding/json"
	"fmt"

	peer "github.com/libp2p/go-libp2p-core/peer"
	pubsub "github.com/libp2p/go-libp2p-pubsub"
)

// --------- //
// Structure //
// --------- //

type Channel struct {
	Message chan *Line
	Event   chan *pubsub.PeerEvent

	Topic *pubsub.Topic

	ctx          *context.Context
	subscription *pubsub.Subscription
	topic_event  *pubsub.TopicEventHandler
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

	topic_event, err := topic.EventHandler()
	if err != nil {
		return nil, err
	}

	channel := &Channel{
		Message: make(chan *Line, 128),
		Event:   make(chan *pubsub.PeerEvent, 128),

		ctx: &ctx,

		Topic:        topic,
		subscription: subscription,
		topic_event:  topic_event,
	}

	go channel.read_messages_forever(&nick_id)
	go channel.read_events_forever()

	return channel, nil
}

func (channel *Channel) Send(ctx context.Context, message *Line) error {
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
		message, err := channel.subscription.Next(*channel.ctx)
		if err != nil {
			return
		}

		new_message := new(Line)
		err = json.Unmarshal(message.Data, new_message)
		if err != nil {
			continue
		}

		channel.Message <- new_message
	}
}

func (channel *Channel) read_events_forever() {
	for {
		event, err := channel.topic_event.NextPeerEvent(*channel.ctx)
		if err != nil {
			fmt.Println("Erreur lors de la lecture d'un évènement", err)
			return
		}
		channel.Event <- &event
	}
}

// -------- //
// Fonction //
// -------- //

func GetTopicName(channel_name *string) string {
	return *channel_name + " (<name> network)"
}
