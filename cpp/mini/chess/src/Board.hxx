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

	Board &operator=(const Board &) = delete;
	Board &operator=(Board &&) = default;

	const CellContent *GetCell(std::size_t x, std::size_t y) const;
	void ClearCell(std::size_t x, std::size_t y);
	void Reset();
	void UpdateCell(std::size_t x, std::size_t y, PieceType pieceType, std::size_t ownerIndex);

	static constexpr std::size_t Width = 8;
	static constexpr std::size_t Height = 8;

	struct CellContent
	{
		PieceType pieceType;
		std::size_t ownerIndex;
	};

private:
	std::optional<CellContent> &AccessCell(std::size_t x, std::size_t y);
	const std::optional<CellContent> &AccessCell(std::size_t x, std::size_t y) const;

	std::array<std::optional<CellContent>, Width * Height> m_cells;
};
