package chat

import (
	"context"
)

// --------- //
// Structure //
// --------- //

type ChatState struct {
	Context *context.Context
	Api     *NodeAPI
}

// -------------- //
// Implémentation //
// -------------- //

func NewState(ctx context.Context, api *NodeAPI) *ChatState {
	state := &ChatState{
		Context: &ctx,
		Api:     api,
	}
	return state
}
