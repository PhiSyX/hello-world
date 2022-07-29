#pragma once

#include <array>
#include <optional>

#include <PieceType.hxx>

class Board
{
public:
	struct CellContent;

	Board();
	Board(const Board &) = delete;
	Board(Board &&) = default;
	~Board() = default;

	const CellContent *GetCell(std::size_t x, std::size_t y) const;

	void Reset();

	Board &operator=(const Board &) = delete;
	Board &operator=(Board &&) = default;

	static constexpr std::size_t Width = 8;
	static constexpr std::size_t Height = 8;

	struct CellContent
	{
		PieceType pieceType;
		std::size_t ownerIndex;
	};

private:
	std::array<std::optional<CellContent>, Width * Height> m_cells;
};
