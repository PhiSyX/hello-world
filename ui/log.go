package ui

import (
	"fmt"

	"github.com/PhiSyX/ibug-p2p-gochat/cli"
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
