package dns

import (
	"context"

	cli "github.com/PhiSyX/ibug-p2p-gochat/cli"
	ui "github.com/PhiSyX/ibug-p2p-gochat/ui"

	host "github.com/libp2p/go-libp2p-core/host"
	peer "github.com/libp2p/go-libp2p-core/peer"
	discovery "github.com/libp2p/go-libp2p/p2p/discovery"
)

// --------- //
// Structure //
// --------- //

type discoveryNotifee struct {
	host *host.Host
	ui   *ui.UI
}

// -------------- //
// Implémentation // -> Interface
// -------------- //

func (notifee *discoveryNotifee) HandlePeerFound(addr peer.AddrInfo) {
	err := (*(notifee.host)).Connect(context.Background(), addr)
	notifee.ui.LogDebug("Multicast DNS pair trouvé : ", addr.ID)
	if err != nil {
		notifee.ui.LogError("Tentative de connexion à un pair échouée : ", err)
		return
	}
}

// -------- //
// Fonction //
// -------- //

func SetupMulticastDNS(ctx *context.Context, host *host.Host, cli_args *cli.CLI, ui *ui.UI) error {
	service, err := discovery.NewMdnsService(*ctx, *host, cli_args.Options.MDNS.Interval, "ibug-p2p-gochat:rendez-vous")
	if err != nil {
		return err
	}

	notifee := &discoveryNotifee{host: host, ui: ui}
	service.RegisterNotifee(notifee)

	return nil
}
