#include <cassert>
#include <utility>

#include <SDL.h>
#include <SDLpp/Renderer.hpp>

namespace SDLpp {
Renderer::Renderer(SDL_Renderer* handle)
  : m_handle(handle)
{
}

Renderer::~Renderer()
{
	if (m_handle) {
		SDL_DestroyRenderer(m_handle);
	}
}

void
Renderer::clear()
{
	assert(m_handle);
	SDL_RenderClear(m_handle);
}

void
Renderer::present()
{
	assert(m_handle);
	SDL_RenderPresent(m_handle);
}

void
Renderer::set_draw_color(std::uint8_t r,
						 std::uint8_t g,
						 std::uint8_t b,
						 std::uint8_t a)
{
	assert(m_handle);
	SDL_SetRenderDrawColor(m_handle, r, g, b, a);
}

void
Renderer::draw_rect(int x, int y, int size)
{
	SDL_Rect dest_rect;

	dest_rect.x = x * size;
	dest_rect.y = y * size;
	dest_rect.w = size;
	dest_rect.h = size;

	SDL_RenderFillRect(m_handle, &dest_rect);
}
}
