#include <iostream>

#include <SDLpp/Lib.hpp>

#include <Game.hpp>

Game::Game(SDLpp::Lib& sdl)
  : m_sdl(sdl)
{
	constexpr int window_width = CASE_WIDTH * TILE_SIZE;
	constexpr int window_height = CASE_HEIGHT * TILE_SIZE;
	m_window = SDLpp::Window(SNAKE_WINDOW_TITLE, window_width, window_height);
	m_renderer = m_window.create_renderer();
	m_apple = Coordinates{ CASE_WIDTH / 2, CASE_HEIGHT / 2 };
}

void
Game::run()
{
	int ticks;

	bool opened = true;
	bool pause = false;

	while (opened) {
		ticks = SDL_GetTicks();

		SDL_Event evt;
		while (m_sdl.poll_event(&evt)) {
			switch (evt.type) {
				case SDL_QUIT: {
					opened = !opened;
				} break;

				case SDL_KEYDOWN: {
					switch (evt.key.keysym.sym) {
						case SDLK_UP: {
							m_snake.change_direction(Direction::Top);
						} break;

						case SDLK_RIGHT: {
							m_snake.change_direction(Direction::Right);
						} break;

						case SDLK_DOWN: {
							m_snake.change_direction(Direction::Bottom);
						} break;

						case SDLK_LEFT: {
							m_snake.change_direction(Direction::Left);
						} break;

						case SDLK_ESCAPE: {
							pause = !pause;
							m_window.update_title(m_snake.len(), pause);
						} break;

						case SDLK_q: {
							int is_ctrl = evt.key.keysym.mod & KMOD_CTRL;
							if (is_ctrl > 0 || pause) {
								opened = false;
							}
						} break;
					}
				} break;

				default:
					break;
			}
		}

		if (!pause) {
			if (m_snake.contains_tail(m_snake.last())) {
				return;
			}

			if (m_snake.does_eat_apple(m_apple)) {
				m_window.update_title(m_snake.len(), false);
			} else {
				m_snake.move();
			}
		}

		draw();

		int new_ticks = SDL_GetTicks();
		int fps = 6;
		SDL_Delay(1000 / fps - new_ticks + ticks);
	}
}

void
Game::draw()
{
	m_renderer.set_draw_color(145, 160, 55, 255);
	m_renderer.clear();
	m_renderer.set_draw_color(205, 20, 50, 255); // Apple
	m_renderer.draw_rect(m_apple.x, m_apple.y, TILE_SIZE);
	m_renderer.set_draw_color(89, 198, 108, 255);

	for (std::size_t n, i = 0; i < m_snake.len() + 1; ++i) {
		n = (m_snake.len() + 1) - i;

		if (n <= 30) {
			m_renderer.set_draw_color(80 + n, 180 + n, 100 + n, 255);
		}

		auto c = m_snake.get(i);
		m_renderer.draw_rect(c.x, c.y, TILE_SIZE);
	}

	m_renderer.present();
}