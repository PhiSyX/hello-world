#pragma once

#include <array>
#include <optional>
#include <vector>

#include <SDLxx/Renderer.hxx>
#include <SDLxx/Texture.hxx>
#include <SDLxx/Window.hxx>

#include <MovementRules/MovementRule.hxx>

#include <Board.hxx>
#include <BoardDrawer.hxx>
#include <Player.hxx>
#include <PieceType.hxx>
#include <Resources.hxx>

namespace SDLxx
{
	class Lib;
}

class ChessGame
{
	friend class Player;

public:
	ChessGame(SDLxx::Lib &sdl);
	ChessGame(const ChessGame &) = delete;
	ChessGame(ChessGame &&) = delete;

	~ChessGame() = default;

	ChessGame &operator=(const ChessGame &) = delete;
	ChessGame &operator=(ChessGame &&) = delete;

	const Board &GetBoard() const;
	const BoardDrawer &GetBoardDrawer() const;

	int Run();

	static constexpr std::size_t PlayerCount = 2;

private:
	bool CheckMovement(PieceType pieceType, std::size_t fromX, std::size_t fromY, std::size_t toX, std::size_t toY) const;
	void InitMovementRules();
	bool MovePiece(std::size_t fromX, std::size_t fromY, std::size_t toX, std::size_t toY);
	void NextTurn();
	void NotifyPieceDeselection();
	void NotifyPieceSelection(std::size_t cellX, std::size_t cellY);

	SDLxx::Lib &m_sdl;
	SDLxx::Window m_window;
	SDLxx::Renderer m_renderer;
	std::vector<Player> m_players;
	Resources m_resources;
	Board m_board;
	std::optional<BoardDrawer> m_boardDrawer;
	std::size_t m_currentPlayer;
	std::array<std::vector<std::unique_ptr<MovementRule>>, PieceCount> m_movementRules;
};
