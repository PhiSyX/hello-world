#include <SDL/Renderer.h>>
#include <SDL/Window.h>
#include <game.h>
#include <snake.h>
#include <stdio.h>

struct Window
create_window(const char* title, usize x, usize y, usize w, usize h)
{
	SDL_Window* handle = SDL_CreateWindow(title, x, y, w, h, 0);
	struct Window window = { .handle = handle };
	return window;
}

struct Renderer
create_renderer(struct Window* window)
{
	SDL_Renderer* handle =
	  SDL_CreateRenderer(window->handle, -1, SDL_RENDERER_ACCELERATED);
	struct Renderer renderer = { .handle = handle };
	return renderer;
}

void
destroy_window(struct Window window)
{
	SDL_DestroyWindow(window.handle);
}

void
update_window_title(struct Window* window, usize score, bool is_pause)
{
	char window_title[255];

	const char* state = is_pause ? "(Pause)" : "";
	sprintf(
	  window_title, "%s - Score: %zu %s", SNAKE_WINDOW_TITLE, score, state);

	SDL_SetWindowTitle(window->handle, window_title);
}