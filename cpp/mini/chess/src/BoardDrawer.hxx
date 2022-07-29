#pragma once

#include <cstddef>

class Board;
class Resources;

namespace SDLxx
{
	class Renderer;
}

class BoardDrawer
{
public:
	BoardDrawer(const Resources &resources, int width, int height);
	BoardDrawer(const BoardDrawer &) = delete;
	BoardDrawer(BoardDrawer &&) = default;

	~BoardDrawer() = default;

	BoardDrawer &operator=(const BoardDrawer &) = delete;
	BoardDrawer &operator=(BoardDrawer &&) = default;

	void Draw(SDLxx::Renderer &renderer, const Board &board);
	static constexpr std::size_t CellSize = 64;

private:
	const Resources &m_resources;
	int m_height;
	int m_width;
};
