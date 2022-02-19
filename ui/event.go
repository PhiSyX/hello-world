package ui

func (ui *UI) handleEvents() {
	for {
		select {
		case <-ui.OkChan:
			return
		}
	}
}
