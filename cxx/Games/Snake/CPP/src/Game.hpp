#pragma once

#include <SDLpp/Renderer.hpp>
#include <SDLpp/Window.hpp>

#include <Snake.hpp>

#define CASE_WIDTH 25
#define CASE_HEIGHT 25
#define TILE_SIZE 25
#define SNAKE_WINDOW_TITLE "Snake Game"

namespace SDLpp {
class Lib;
}

class Game
{

  public:
	Game(SDLpp::Lib& sdl);
	Game(const Game&) = delete;
	Game(Game&&) = delete;
	~Game() = default;

	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;

	void run();
	void draw();

  private:
	SDLpp::Lib& m_sdl;
	SDLpp::Window m_window;
	SDLpp::Renderer m_renderer;

	Coordinates m_apple;
	Snake m_snake;
};