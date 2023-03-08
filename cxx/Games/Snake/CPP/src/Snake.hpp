#pragma once

#include <bitset>
#include <vector>

enum class Direction
{
	Top,
	Right,
	Bottom,
	Left,
};

struct Coordinates
{
	std::size_t x;
	std::size_t y;

	inline void decrement_x(std::size_t min, std::size_t max)
	{
		x = x <= min ? max : x - 1;
	}

	inline void decrement_y(std::size_t min, std::size_t max)
	{
		y = y <= min ? max : y - 1;
	}

	inline void increment_x(std::size_t min, std::size_t max)
	{
		x = x >= max ? min : x + 1;
	}

	inline void increment_y(std::size_t min, std::size_t max)
	{
		y = y >= max ? min : y + 1;
	}
};

class Snake
{
  public:
	Snake();
	Snake(const Snake&) = delete;
	Snake(Snake&&) = delete;
	~Snake() = default;

	std::size_t len();

	void change_direction(Direction direction);
	bool contains_tail(Coordinates& coordinates);
	Coordinates& get(std::size_t pos);
	Coordinates& last();
	bool does_eat_apple(Coordinates& apple);
	void move();

  private:
	std::vector<Coordinates> m_body;
	Direction m_direction;

	bool is_head(const Coordinates& coordinates);
	void move_head();
	void move_apple(Coordinates& apple);
	void move_tail();
};