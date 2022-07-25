#include <iostream>

#include <SDLxx/Lib.hxx>
#include <ChessGame.hxx>

int main(int argc, char **argv)
{
	try
	{
		SDLxx::Lib sdl;
		ChessGame game(sdl);
		return game.Run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 0;
	}
}
