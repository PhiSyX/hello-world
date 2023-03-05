#pragma once

#include <SDL.h>
#include <cstdint>
#include <memory>
#include <string>

#include <MovablePtr.hpp>

namespace SDLpp {
class Window;

class Renderer
{
  public:
	Renderer() = default;
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&& renderer) noexcept = default;
	~Renderer();

	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&& renderer) noexcept = default;

	void clear();
	void present();
	void set_draw_color(std::uint8_t r,
						std::uint8_t g,
						std::uint8_t b,
						std::uint8_t a);

	void draw_rect(int x, int y, int tile_size);

  private:
	friend Window;

	explicit Renderer(SDL_Renderer* handle);

	MovablePtr<SDL_Renderer> m_handle;
};
}