#include <stdbool.h>
#include <stdio.h>

#include <game.h>

struct Game
create_game(void)
{
	SDL_Init(SDL_INIT_VIDEO);

	struct Window window = create_window(SNAKE_WINDOW_TITLE,
										 SDL_WINDOWPOS_CENTERED,
										 SDL_WINDOWPOS_CENTERED,
										 SNAKE_CASE_WIDTH,
										 SNAKE_CASE_HEIGHT);
	struct Renderer renderer = create_renderer(&window);

	struct Game game = {
		.window = window,
		.renderer = renderer,
		.snake = create_snake(),
		.apple = create_apple(CASE_WIDTH / 2, CASE_HEIGHT / 2),
	};

	return game;
}

static void // FIXME
draw_from_coordinates(struct Renderer renderer, struct Coordinates* coordinates)
{
	SDL_Rect dest_rect;

	dest_rect.x = coordinates->x * TILE_SIZE;
	dest_rect.y = coordinates->y * TILE_SIZE;
	dest_rect.w = TILE_SIZE;
	dest_rect.h = TILE_SIZE;

	SDL_RenderFillRect(renderer.handle, &dest_rect);
}

static void
draw_map(struct Renderer renderer,
		 struct Snake* snake,
		 struct Coordinates* apple)
{
	set_draw_color(renderer, 145, 160, 55, 255);
	clear_render(renderer);
	set_draw_color(renderer, 205, 20, 50, 255); // Apple
	draw_from_coordinates(renderer, apple);
	set_draw_color(renderer, 89, 198, 108, 255);

	for (usize n, i = 0; i < snake->length; ++i) {
		n = snake->length - i;

		if (n <= 30) {
			set_draw_color(renderer, 80 + n, 180 + n, 100 + n, 255);
		}

		draw_from_coordinates(renderer, &snake->body[i]);
	}

	present_render(renderer);
}

void
run_game(struct Game* game)
{
	int ticks;

	bool opened = true;
	bool pause = false;

	while (opened) {
		ticks = SDL_GetTicks();

		SDL_Event evt;
		while (SDL_PollEvent(&evt) > 0) {
			switch (evt.type) {
				case SDL_QUIT: {
					opened = !opened;
				} break;

				case SDL_KEYDOWN: {
					switch (evt.key.keysym.sym) {
						case SDLK_UP: {
							change_snake_direction(&game->snake, DirectionTop);
						} break;

						case SDLK_RIGHT: {
							change_snake_direction(&game->snake,
												   DirectionRight);
						} break;

						case SDLK_DOWN: {
							change_snake_direction(&game->snake,
												   DirectionBottom);
						} break;

						case SDLK_LEFT: {
							change_snake_direction(&game->snake, DirectionLeft);
						} break;

						case SDLK_ESCAPE: {
							pause = !pause;
							update_window_title(
							  &game->window, game->snake.length - 1, pause);
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
			if (contains_tail_snake(
				  &game->snake, &game->snake.body[game->snake.length - 1])) {
				return;
			}

			if (does_snake_eat_apple(&game->snake, &game->apple)) {
				update_window_title(
				  &game->window, game->snake.length - 1, false);
			} else {
				move_snake(&game->snake);
			}
		}

		draw_map(game->renderer, &game->snake, &game->apple);

		int new_ticks = SDL_GetTicks();
		int fps = 6;
		SDL_Delay(1000 / fps - new_ticks + ticks);
	}
}

void
destroy_game(struct Game game)
{
	destroy_snake(game.snake);
	destroy_window(game.window);
	destroy_renderer(game.renderer);

	SDL_Quit();
}