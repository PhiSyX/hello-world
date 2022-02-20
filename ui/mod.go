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

	app      *view.Application
	channel  *chat.Channel
	history  *UIHistory
	nicklist *UINicklist
	input    *UIInput
	state    *chat.ChatState

	OkChan chan struct{}
}

type UIHistory struct {
	area *view.TextView
	list []*string
}

type UIInput struct {
	model chan *string
	field *view.Flex
}

type UINicklist struct {
	area *view.TextView
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
	ui.input.field.SetInputCapture(func(evt *cell.EventKey) *cell.EventKey {
		if evt.Key() == cell.KeyTAB {
			ui.app.SetFocus(ui.history.area)
		}
		return evt
	})

	ui.history.area.SetInputCapture(func(evt *cell.EventKey) *cell.EventKey {
		if evt.Key() == cell.KeyTAB {
			ui.app.SetFocus(ui.nicklist.area)
		}

		return evt
	})

	ui.nicklist.area.SetInputCapture(func(evt *cell.EventKey) *cell.EventKey {
		if evt.Key() == cell.KeyTAB {
			ui.app.SetFocus(ui.input.field)
		}
		return evt
	})
}

// -------- //
// Fonction //
// -------- //

func CreateUIFromCLI(cli_args *cli.CLI) *UI {
	app := view.NewApplication()

	input_model := make(chan *string, 32)
	input_area := build_input_field_area(cli_args.Options.Nick, input_model)
	history_area := build_history_area(app.Draw)
	nicklist_area := build_nicklist_area(app.Draw)
	chat_area := merge_history_and_nicklist_areas(history_area, nicklist_area)
	root := merge_chat_and_input_areas(chat_area, input_area)

	app.SetRoot(root, true)

	ui := &UI{
		cli_args: cli_args,

		app: app,

		history: &UIHistory{
			area: history_area,
			list: []*string{},
		},

		input: &UIInput{
			model: input_model,
			field: input_area,
		},

		nicklist: &UINicklist{
			area: nicklist_area,
		},

		OkChan: make(chan struct{}, 1),
	}

	ui.on_input_capture()

	return ui
}

func build_input_field_area(nick *string, model chan *string) *view.Flex {
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
		AddItem(field, 0, 1, !mode_normal)
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
