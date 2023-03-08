#include <algorithm>
#include <iostream>
#include <ranges>

#include <Game.hpp>
#include <Snake.hpp>

Snake::Snake()
  : m_direction(Direction::Right)
{
	auto coord = Coordinates{ 1, CASE_HEIGHT * TILE_SIZE / 2 };
	m_body.push_back(coord);
}

std::size_t
Snake::len()
{
	return m_body.size() - 1;
}

void
Snake::change_direction(Direction direction)
{
	m_direction = direction;
}

bool
Snake::contains_tail(Coordinates& coordinates)
{
	for (std::size_t i = 0; i < len(); ++i) {
		if (coordinates.x == m_body[i].x && coordinates.y == m_body[i].y) {
			return true;
		}
	}

	return false;
}

bool
Snake::does_eat_apple(Coordinates& apple)
{
	if (!is_head(apple)) {
		return false;
	}

	m_body.push_back(m_body[m_body.size() - 1]);

	move_head();
	move_apple(apple);

	return true;
}

Coordinates&
Snake::get(std::size_t pos)
{
	return m_body.at(pos);
}

Coordinates&
Snake::last()
{
	return m_body.back();
}

bool
Snake::is_head(const Coordinates& coordinates)
{
	const auto& head = last();
	return head.x == coordinates.x && head.y == coordinates.y;
}

void
Snake::move_head()
{
	auto& head = last();

	switch (m_direction) {
		case Direction::Top: {
			head.decrement_y(0, CASE_HEIGHT - 1);
		} break;

		case Direction::Left: {
			head.decrement_x(0, CASE_HEIGHT - 1);
		} break;

		case Direction::Bottom: {
			head.increment_y(0, CASE_HEIGHT - 1);
		} break;

		case Direction::Right: {
			head.increment_x(0, CASE_WIDTH - 1);
		} break;
	}
}

void
Snake::move_apple(Coordinates& apple)
{
	do {
		apple.x = rand() % CASE_WIDTH;
		apple.y = rand() % CASE_HEIGHT;
	} while (contains_tail(apple) || is_head(apple));
}

void
Snake::move_tail()
{
	for (std::size_t i = 0; i < len(); ++i) {
		m_body[i] = m_body[i + 1];
	}
}

void
Snake::move()
{
	move_tail();
	move_head();
}
