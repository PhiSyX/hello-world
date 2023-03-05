#include <iostream>

#include <SDLpp/Lib.hpp>

#include <Game.hpp>

int
main()
{
	try {
		SDLpp::Lib sdl;
		Game game(sdl);
		game.run();
		return EXIT_SUCCESS;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}