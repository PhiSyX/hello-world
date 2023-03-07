#ifndef SNAKE_H
#define SNAKE_H

enum Direction
{
	DirectionTop,
	DirectionRight,
	DirectionBottom,
	DirectionLeft,
};

struct Coordinates
{
	usize x;
	usize y;
};

struct Snake
{
	struct Coordinates* body;
	usize length;
	enum Direction direction;
};

struct Snake
create_snake(void);

void
change_snake_direction(struct Snake*, enum Direction);

struct Coordinates
create_apple(usize x, usize y);

void destroy_snake(struct Snake);

bool
contains_tail_snake(const struct Snake* snake,
					const struct Coordinates* coordinates);

bool
does_snake_eat_apple(struct Snake* snake, struct Coordinates* apple);

void
move_snake(struct Snake* snake);

#endif