#include <stdlib.h>

#include <game.h>
#include <snake.h>

struct Snake
create_snake(void)
{
	struct Snake snake = { .body = malloc(sizeof(struct Coordinates)),
						   .length = 1,
						   .direction = DirectionRight };

	snake.body[0] = (struct Coordinates){ .y = SNAKE_CASE_HEIGHT / 2 };

	return snake;
}

struct Coordinates
create_apple(usize x, usize y)
{
	struct Coordinates coord = { .x = x, .y = y };
	return coord;
}

void
change_snake_direction(struct Snake* snake, enum Direction direction)
{
	snake->direction = direction;
}

void
destroy_snake(struct Snake snake)
{
	free(snake.body);
}

bool
contains_tail_snake(const struct Snake* snake,
					const struct Coordinates* coordinates)
{
	for (usize i = 0; i < snake->length - 1; ++i) {
		if (coordinates->x == snake->body[i].x &&
			coordinates->y == snake->body[i].y) {
			return true;
		}
	}

	return false;
}

static inline void
_increment(usize* n, usize min, usize max)
{
	*n = *n >= max ? min : *n + 1;
}

static inline void
_decrement(usize* n, usize min, usize max)
{
	*n = *n <= min ? max : *n - 1;
}

static bool
is_head(const struct Snake* snake, const struct Coordinates* coordinates)
{
	const struct Coordinates head = snake->body[snake->length - 1];
	return head.x == coordinates->x && head.y == coordinates->y;
}

static void
move_head(struct Snake* snake)
{
	struct Coordinates* head = &snake->body[snake->length - 1];

	switch (snake->direction) {
		case DirectionTop: {
			_decrement(&head->y, 0, CASE_HEIGHT - 1);
		} break;

		case DirectionLeft: {
			_decrement(&head->x, 0, CASE_WIDTH - 1);
		} break;

		case DirectionBottom: {
			_increment(&head->y, 0, CASE_HEIGHT - 1);
		} break;

		case DirectionRight: {
			_increment(&head->x, 0, CASE_WIDTH - 1);
		} break;
	}
}

static void
move_apple(const struct Snake* snake, struct Coordinates* apple)
{
	do {
		apple->x = rand() % CASE_WIDTH;
		apple->y = rand() % CASE_HEIGHT;
	} while (contains_tail_snake(snake, apple) || is_head(snake, apple));
}

static void
move_tail(struct Snake* snake)
{
	for (usize i = 0; i < snake->length - 1; ++i) {
		snake->body[i] = snake->body[i + 1];
	}
}

bool
does_snake_eat_apple(struct Snake* snake, struct Coordinates* apple)
{
	if (!is_head(snake, apple)) {
		return false;
	}

	snake->body =
	  realloc(snake->body, ++snake->length * sizeof(struct Coordinates));

	snake->body[snake->length - 1] = snake->body[snake->length - 2];

	move_head(snake);
	move_apple(snake, apple);

	return true;
}

void
move_snake(struct Snake* snake)
{
	move_tail(snake);
	move_head(snake);
}