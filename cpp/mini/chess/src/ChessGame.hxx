#pragma once

#include <optional>
#include <vector>

#include <SDLxx/Renderer.hxx>
#include <SDLxx/Texture.hxx>
#include <SDLxx/Window.hxx>
#include <Board.hxx>
#include <BoardDrawer.hxx>
#include <Player.hxx>
#include <Resources.hxx>

namespace SDLxx
{
	class Lib;
}

class ChessGame
{
public:
	ChessGame(SDLxx::Lib &sdl);
	ChessGame(const ChessGame &) = delete;
	ChessGame(ChessGame &&) = delete;
	~ChessGame() = default;

	int Run();

	ChessGame &operator=(const ChessGame &) = delete;
	ChessGame &operator=(ChessGame &&) = delete;

private:
	SDLxx::Lib &m_sdl;
	SDLxx::Window m_window;
	SDLxx::Renderer m_renderer;
	std::vector<Player> m_players;
	Resources m_resources;
	Board m_board;
	std::optional<BoardDrawer> m_boardDrawer;
};
