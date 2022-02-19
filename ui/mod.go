package ui

import (
	"log"

	"github.com/PhiSyX/ibug-p2p-gochat/chat"
	"github.com/PhiSyX/ibug-p2p-gochat/cli"

	"github.com/gdamore/tcell/v2"
	"github.com/rivo/tview"
)

// --------- //
// Structure //
// --------- //

type UI struct {
	app     *tview.Application
	channel *chat.Channel
	history *UIHistory
	input   *UIInput
	state   *chat.ChatState

	OkChan chan struct{}
}

type UIHistory struct {
	area *tview.TextView
	list []*string
}

type UIInput struct {
	model chan *string
	field *tview.Flex
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
	go ui.handleEvents()
}

func (ui *UI) end() {
	ui.OkChan <- struct{}{}
	ui.app.Stop()
}

// -------- //
// Fonction //
// -------- //

func CreateUIFromCLI(cli_args *cli.CLI) *UI {
	app := tview.NewApplication()

	input_model := make(chan *string, 32)
	input_area := build_input_field_area(cli_args.Options.Nick, input_model)
	history_area := build_history_area(app.Draw)
	nicklist_area := build_nicklist_area(app.Draw)
	chat_area := merge_history_and_nicklist_areas(history_area, nicklist_area)
	root := merge_chat_and_input_areas(chat_area, input_area)

	app.SetRoot(root, true)

	ui := &UI{
		app: app,

		history: &UIHistory{
			area: history_area,
			list: []*string{},
		},

		input: &UIInput{
			model: input_model,
			field: input_area,
		},

		OkChan: make(chan struct{}, 1),
	}

	return ui
}

func build_input_field_area(nick *string, model chan *string) *tview.Flex {
	area := tview.NewFlex()

	// TODO: VIM mode
	mode_normal := false           // TODO: true
	mode_color := tcell.ColorGreen // TODO: tcell.ColorRed

	nickname := tview.NewTextView().
		SetText(*nick).
		SetTextColor(tcell.ColorBlue)

	field := tview.NewInputField().
		SetLabelColor(mode_color).
		SetLabel(" λ ").
		SetFieldBackgroundColor(tcell.ColorWhite).
		SetFieldTextColor(tcell.ColorBlack).
		SetFieldWidth(0)

	field.SetDoneFunc(func(key tcell.Key) {
		if key != tcell.KeyEnter {
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

func build_history_area(fn func() *tview.Application) *tview.TextView {
	history_area := tview.NewTextView()
	history_area.SetScrollable(true).SetBorder(true)
	history_area.SetDynamicColors(true)

	history_area.SetChangedFunc(func() {
		fn()
	})

	return history_area
}

func build_nicklist_area(fn func() *tview.Application) *tview.TextView {
	nicklist_area := tview.NewTextView()
	nicklist_area.SetScrollable(true).SetBorder(true).SetTitle(" Utilisateurs ")
	return nicklist_area
}

func merge_history_and_nicklist_areas(history_area *tview.TextView, nicklist_area *tview.TextView) *tview.Flex {
	chat_area := tview.NewFlex().
		AddItem(history_area, 0, 1, false).
		AddItem(nicklist_area, 30, 1, false)
	return chat_area
}

func merge_chat_and_input_areas(chat_area *tview.Flex, input_area *tview.Flex) *tview.Flex {
	root := tview.NewFlex().SetDirection(tview.FlexRow).
		AddItem(chat_area, 0, 1, false).
		AddItem(input_area, 1, 1, true)
	return root
}
