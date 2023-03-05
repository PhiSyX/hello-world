#include <SDL.h>
#include <SDLpp/Window.hpp>
#include <cassert>
#include <stdexcept>
#include <utility>

namespace SDLpp {
Window::Window(const std::string& title, int w, int h)
  : Window(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h)
{
}

Window::Window(const std::string& title, int x, int y, int w, int h)
  : Window(title, x, y, w, h, 0)
{
}

Window::Window(const std::string& title,
			   int x,
			   int y,
			   int w,
			   int h,
			   std::uint32_t flags)
{
	m_handle = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
	if (!m_handle) {
		throw std::runtime_error(
		  std::string("impossible de créer la fenêtre: ") + SDL_GetError());
	}
}

Window::~Window()
{
	if (m_handle) {
		SDL_DestroyWindow(m_handle);
	}
}

Renderer
Window::create_renderer()
{
	assert(m_handle);

	SDL_Renderer* renderer = SDL_CreateRenderer(m_handle, -1, 0);
	if (!renderer) {
		throw std::runtime_error(
		  std::string("impossible de créer le moteur de rendu: ") +
		  SDL_GetError());
	}

	return Renderer(renderer);
}

void
Window::update_title(std::size_t score, bool is_pause)
{
	char window_title[255];
	const char* state = is_pause ? "(Pause)" : "";
	sprintf(window_title, "%s - Score: %zu %s", "Snake Game", score, state);
	SDL_SetWindowTitle(m_handle, window_title);
}
}
