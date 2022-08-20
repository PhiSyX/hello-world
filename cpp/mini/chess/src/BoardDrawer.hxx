#pragma once

#include <bitset>
#include <cstddef>
#include <optional>
#include <vector>

#include <SDL.h>

#include <Board.hxx> //< TODO: Replace by Constants.hpp

class Resources;

namespace SDLxx
{
	class Renderer;
	class Sprite;
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

	void ClearMovementOverlay();
	void ClearSelectionOverlay();
	void Draw(SDLxx::Renderer &renderer, const Board &board) const;
	void EnableMovementOverlay(std::size_t cellX, std::size_t cellY, bool enable = true);
	void EnableSelectionOverlay(std::size_t cellX, std::size_t cellY);
	bool GetHoveringCell(int x, int y, std::size_t &cellX, std::size_t &cellY) const;

	static constexpr std::size_t CellSize = 64;

	static void GetDrawPosition(std::size_t cellX, std::size_t cellY, float &xOffset, float &yOffset);

private:
	struct SelectionOverlay
	{
		std::size_t cellX;
		std::size_t cellY;
	};

	struct DrawSprite
	{
		const SDLxx::Sprite *sprite;
		float drawX;
		float drawY;
		SDL_RendererFlip flipFlags = SDL_FLIP_NONE;
		int order = 0;
	};

	std::bitset<Board::Width * Board::Height> m_overlaidCells;
	std::optional<SelectionOverlay> m_selectionOverlay;
	mutable std::vector<DrawSprite> m_drawSprites;
	const Resources &m_resources;
	int m_height;
	int m_width;
};
