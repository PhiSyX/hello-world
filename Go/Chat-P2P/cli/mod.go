package cli

import (
	"flag"
	"os"
	"strings"
	"time"
)

// --------- //
// Structure //
// --------- //

type CLI struct {
	Options *CLIOptions
}

type CLIOptions struct {
	Nick    *string
	Channel *string
	Debug   *bool
	MDNS    *ConfigMulticastDNS
}

// -------- //
// Fonction //
// -------- //

func GetCliArgs() *CLI {
	user_env, user_env_exists := os.LookupEnv("USERNAME")
	if !user_env_exists {
		user_env, _ = os.LookupEnv("USER")
	}

	nick := flag.String("nick", user_env, "Pseudonyme qui sera utilisé pour le chat")
	channel := flag.String("channel", "#ibug", "Salon à rejoindre à la connexion du serveur")
	debug := flag.Bool("debug", false, "Affiche plus d'informations")
	flag.Parse()

	if len(strings.TrimSpace(*nick)) == 0 {
		panic("Un pseudo est requis")
	}

	options := &CLIOptions{
		Nick:    nick,
		Channel: channel,
		Debug:   debug,
		MDNS: &ConfigMulticastDNS{
			Interval: time.Duration(900) * time.Second,
		},
	}

	cli := &CLI{
		Options: options,
	}

	return cli
}
