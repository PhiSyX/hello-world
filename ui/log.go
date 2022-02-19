package ui

import (
	"fmt"

	chat "github.com/PhiSyX/ibug-p2p-gochat/chat"
	cli "github.com/PhiSyX/ibug-p2p-gochat/cli"
	pubsub "github.com/libp2p/go-libp2p-pubsub"
)

// -------------- //
// Implémentation //
// -------------- //

func (ui *UI) LogInfo(args ...interface{}) {
	if !*ui.cli_args.Options.Debug {
		return
	}

	prompt := cli.Color("blue", "*  INFO: ")
	fmt.Fprintln(ui.history.area, prompt, fmt.Sprint(args...))
}

func (ui *UI) LogTrace(args ...interface{}) {
	if !*ui.cli_args.Options.Debug {
		return
	}

	prompt := cli.Color("gray", "* TRACE: ")
	fmt.Fprintln(ui.history.area, prompt, fmt.Sprint(args...))
}

func (ui *UI) LogDebug(args ...interface{}) {
	if !*ui.cli_args.Options.Debug {
		return
	}

	prompt := cli.Color("purple", "* DEBUG: ")
	fmt.Fprintln(ui.history.area, prompt, fmt.Sprint(args...))
}

func (ui *UI) LogError(args ...interface{}) {
	if !*ui.cli_args.Options.Debug {
		return
	}

	prompt := cli.Color("darkred", "* ERROR: ")
	fmt.Fprintln(ui.history.area, prompt, fmt.Sprint(args...))
}

func (ui *UI) echo_event(evt *pubsub.PeerEvent) {
	switch evt.Type {
	case pubsub.PeerJoin:
		ui.echo_event_join(evt.Peer.Pretty())
	case pubsub.PeerLeave:
		ui.echo_event_part(evt.Peer.Pretty())
	}
}

func (ui *UI) echo_event_join(nick string) {
	prompt := cli.Color("green", "*  JOIN: ")
	fmt.Fprintln(ui.history.area, prompt, nick)
}

func (ui *UI) echo_event_part(nick string) {
	prompt := cli.Color("red", "*  PART: ")
	fmt.Fprintln(ui.history.area, prompt, nick)
}

func (ui *UI) echo_message(line *chat.Message) {
	var message_color string

	if line.Sender.ID.Pretty() == ui.state.Sender.ID.Pretty() {
		message_color = "blue"
	} else {
		message_color = "white"
	}

	if len(line.Sender.Nick) < 1 {
		line.Sender.Nick = "Anonymous"
	}

	prompt := cli.Color(message_color, fmt.Sprintf("<%s>", line.Sender.Nick))
	fmt.Fprintln(ui.history.area, prompt, line.Text)
}
