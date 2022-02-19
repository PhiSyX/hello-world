package ui

import (
	"context"
	"strings"

	chat "github.com/PhiSyX/ibug-p2p-gochat/chat"
)

func (ui *UI) handle_events() {
	for {
		select {
		case input := <-ui.input.model:
			message := &chat.Line{
				Message: *input,
				Sender:  *ui.state.Sender,
			}

			err := ui.channel.Send(context.Background(), message)

			if err != nil {
				ui.LogError("envoie de message: ", err)
			}

		case line := <-ui.channel.Message:
			if line != nil {
				ui.echo_message(line)
			}

		case evt := <-ui.channel.Event:
			if evt != nil {
				ui.echo_event(evt)
				ui.refresh_nicklist()
			}

		case <-ui.OkChan:
			return
		}
	}
}

func (ui *UI) refresh_nicklist() {
	nicklist := ui.channel.Topic.ListPeers()
	nicklist_ids := make([]string, len(nicklist))
	for i, nick := range nicklist {
		nicklist_ids[i] = nick.String()
	}
	ui.nicklist.area.SetText(strings.Join(nicklist_ids, "\n"))
	ui.app.Draw()
}
