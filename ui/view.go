package ui

import "fmt"

// -------------- //
// Implémentation //
// -------------- //

func (ui *UI) set_topic() {
	ui.history.area.SetTitle(fmt.Sprintf(" Salon: %s ", ui.channel.Topic.String()))
}
