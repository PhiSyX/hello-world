#ifndef GAME_H
#define GAME_H

#include <SDL/Renderer.h>
#include <SDL/Window.h>
#include <game.h>
#include <snake.h>

#define CASE_WIDTH 25
#define CASE_HEIGHT 25
#define TILE_SIZE 25

#define SNAKE_CASE_WIDTH CASE_WIDTH* TILE_SIZE
#define SNAKE_CASE_HEIGHT CASE_HEIGHT* TILE_SIZE

#define SNAKE_WINDOW_TITLE "Snake Game"

struct Game
{
	struct Window window;
	struct Renderer renderer;
	struct Snake snake;
	struct Coordinates apple;
};

struct Game
create_game(void);

void
run_game(struct Game*);

void
destroy_game(struct Game game);

#endif