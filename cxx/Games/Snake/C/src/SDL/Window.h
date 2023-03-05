#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <SDL.h>
#include <stdbool.h>

typedef size_t usize;

struct Window
{
	SDL_Window* handle;
};

struct Window
create_window(const char* title, usize x, usize y, usize w, usize h);

struct Renderer
create_renderer(struct Window*);

void destroy_window(struct Window);

void
update_window_title(struct Window*, usize score, bool is_pause);

#endif