#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include <SDL.h>

struct Renderer
{
	SDL_Renderer* handle;
};

void destroy_renderer(struct Renderer);

void clear_render(struct Renderer);
void present_render(struct Renderer);

void
set_draw_color(struct Renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

#endif