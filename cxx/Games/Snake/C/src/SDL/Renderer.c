#include <SDL/Renderer.h>

void
destroy_renderer(struct Renderer renderer)
{
	if (renderer.handle) {
		SDL_DestroyRenderer(renderer.handle);
	}
}

void
clear_render(struct Renderer renderer)
{
	SDL_RenderClear(renderer.handle);
}

void
present_render(struct Renderer renderer)
{
	SDL_RenderPresent(renderer.handle);
}

void
set_draw_color(struct Renderer renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer.handle, r, g, b, a);
}