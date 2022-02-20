package ui

import (
	"context"
	"fmt"
	"runtime"
	"strings"

	chat "github.com/PhiSyX/ibug-p2p-gochat/chat"
	mm "github.com/PhiSyX/ibug-p2p-gochat/mm"
)

func (ui *UI) handle_events() {
	for {
		select {
		case input := <-ui.input.model:
			if len(ui.input.history) == 0 || *ui.input.history[len(ui.input.history)-1] != *input {
				ui.input.history = append(ui.input.history, input)
			}

			// Commandes
			if strings.HasPrefix(*input, "/") {
				ui.handle_command(strings.TrimPrefix(*input, "/"))
				continue
			}

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
	ui.areas.nicklist.SetText(strings.Join(nicklist_ids, "\n"))
	ui.app.Draw()
}

func (ui *UI) handle_command(command string) {
	args := strings.Split(command, " ")

	switch args[0] {
	case "help":
		ui.Log(`Commandes:
			/clear : efface le contenu de la fenêtre de chat (libère la mémoire)
			/exit : quitte le programme
		`)

	case "clear":
		memstats := mm.NewMemory()
		runtime.GC()
		ui.areas.history.Clear()
		new_memstats := mm.NewMemory()
		ui.Log(fmt.Sprintf("%fMio de mémoire libérée", memstats.Alloc-new_memstats.Alloc))

	case "exit":
		ui.end()

	default:
		ui.Log("Commande inconnue, utilisez `/help` pour lister les commandes disponibles")
	}
}
