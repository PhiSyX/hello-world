#pragma once

#include <MovablePtr.hpp>
#include <SDLpp/Renderer.hpp>
#include <cstdint>
#include <string>

struct SDL_Window;

namespace SDLpp {
class Window
{
  public:
	Window() = default;
	Window(const std::string& title, int width, int height);
	Window(const std::string& title, int x, int y, int width, int height);
	Window(const std::string& title,
		   int x,
		   int y,
		   int width,
		   int height,
		   std::uint32_t flags);
	Window(const Window&) = delete;
	Window(Window&& window) noexcept = default;
	~Window();

	Window& operator=(const Window&) = delete;
	Window& operator=(Window&& window) noexcept = default;

	Renderer create_renderer();

	void update_title(std::size_t score, bool is_pause);

  private:
	MovablePtr<SDL_Window> m_handle;
};
}
