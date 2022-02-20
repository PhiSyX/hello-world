package ui

import (
	"log"

	chat "github.com/PhiSyX/ibug-p2p-gochat/chat"
	cli "github.com/PhiSyX/ibug-p2p-gochat/cli"

	cell "github.com/gdamore/tcell/v2"
	view "github.com/rivo/tview"
)

// --------- //
// Structure //
// --------- //

type UI struct {
	cli_args *cli.CLI

	app     *view.Application
	channel *chat.Channel
	areas   *UIArea
	input   *UIInput
	state   *chat.ChatState

	OkChan chan struct{}
}

type UIArea struct {
	input    *view.Flex
	history  *view.TextView
	nicklist *view.TextView
}

type UIInput struct {
	model       chan *string
	field       *view.InputField
	history     []*string
	current_idx int
}

type UINicklist struct {
}

// -------------- //
// Implémentation //
// -------------- //

func (ui *UI) Run() {
	defer ui.end()
	err := ui.app.Run()
	if err != nil {
		log.Panicln("Erreur lors du lancement de l'application :", err)
	}
}

func (ui *UI) Start(state *chat.ChatState, channel *chat.Channel) {
	ui.state = state
	ui.channel = channel
	ui.set_topic()
	go ui.handle_events()
}

func (ui *UI) end() {
	ui.OkChan <- struct{}{}
	ui.app.Stop()
}

func (ui *UI) on_input_capture() {
	ui.areas.input.SetInputCapture(func(evt *cell.EventKey) *cell.EventKey {
		if evt.Key() == cell.KeyTAB {
			ui.app.SetFocus(ui.areas.history)
		} else if evt.Key() == cell.KeyUp || evt.Key() == cell.KeyDown {
			ui.handle_input_history(evt)
		}

		return evt
	})

	ui.areas.history.SetInputCapture(func(evt *cell.EventKey) *cell.EventKey {
		if evt.Key() == cell.KeyTAB {
			ui.app.SetFocus(ui.areas.nicklist)
		}

		return evt
	})

	ui.areas.nicklist.SetInputCapture(func(evt *cell.EventKey) *cell.EventKey {
		if evt.Key() == cell.KeyTAB {
			ui.app.SetFocus(ui.areas.input)
		}
		return evt
	})
}

func (ui *UI) handle_input_history(evt *cell.EventKey) {
	switch evt.Key() {
	case cell.KeyUp:
		if ui.input.current_idx >= len(ui.input.history) || ui.input.current_idx < 0 {
			return
		}

		ui.input.current_idx++
		ui.input.field.SetText(*ui.input.history[ui.input.current_idx-1])

	case cell.KeyDown:
		if ui.input.current_idx <= 1 {
			ui.input.current_idx = 0
			ui.input.field.SetText("")
			return
		}

		ui.input.current_idx--
		ui.input.field.SetText(*ui.input.history[ui.input.current_idx])

	default:
		ui.input.current_idx = 0
	}
}

// -------- //
// Fonction //
// -------- //

func CreateUIFromCLI(cli_args *cli.CLI) *UI {
	app := view.NewApplication()

	input_model := make(chan *string, 32)
	input_area, input_field := build_input_field_area(cli_args.Options.Nick, input_model)
	history_area := build_history_area(app.Draw)
	nicklist_area := build_nicklist_area(app.Draw)
	chat_area := merge_history_and_nicklist_areas(history_area, nicklist_area)
	root := merge_chat_and_input_areas(chat_area, input_area)

	app.SetRoot(root, true).EnableMouse(true)

	ui := &UI{
		cli_args: cli_args,

		app: app,

		areas: &UIArea{
			input:    input_area,
			history:  history_area,
			nicklist: nicklist_area,
		},

		input: &UIInput{
			model: input_model,
			field: input_field,
		},

		OkChan: make(chan struct{}, 1),
	}

	ui.on_input_capture()

	return ui
}

func build_input_field_area(nick *string, model chan *string) (*view.Flex, *view.InputField) {
	area := view.NewFlex()

	// TODO: VIM mode
	mode_normal := false          // TODO: true
	mode_color := cell.ColorGreen // TODO: cell.ColorRed

	nickname := view.NewTextView().
		SetText(*nick).
		SetTextColor(cell.ColorBlue)

	field := view.NewInputField().
		SetLabelColor(mode_color).
		SetLabel(" λ ").
		SetFieldBackgroundColor(cell.ColorWhite).
		SetFieldTextColor(cell.ColorBlack).
		SetFieldWidth(0)

	field.SetDoneFunc(func(key cell.Key) {
		if key != cell.KeyEnter {
			return
		}

		line := field.GetText()
		if len(line) == 0 {
			return
		}

		model <- &line
		field.SetText("")
	})

	return area.
		AddItem(nickname, 6, 0, false).
		AddItem(field, 0, 1, !mode_normal), field
}

func build_history_area(fn func() *view.Application) *view.TextView {
	history_area := view.NewTextView()
	history_area.SetScrollable(true).SetBorder(true)
	history_area.SetDynamicColors(true)

	history_area.SetChangedFunc(func() {
		fn()
	})

	return history_area
}

func build_nicklist_area(fn func() *view.Application) *view.TextView {
	nicklist_area := view.NewTextView()
	nicklist_area.SetScrollable(true).SetBorder(true).SetTitle(" Utilisateurs ")
	return nicklist_area
}

func merge_history_and_nicklist_areas(history_area *view.TextView, nicklist_area *view.TextView) *view.Flex {
	chat_area := view.NewFlex().
		AddItem(history_area, 0, 1, false).
		AddItem(nicklist_area, 30, 1, false)
	return chat_area
}

func merge_chat_and_input_areas(chat_area *view.Flex, input_area *view.Flex) *view.Flex {
	root := view.NewFlex().SetDirection(view.FlexRow).
		AddItem(chat_area, 0, 1, false).
		AddItem(input_area, 1, 1, true)
	return root
}
