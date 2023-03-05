#include <SDL.h>
#include <SDLpp/Lib.hpp>

namespace SDLpp {
Lib::Lib()
{
	SDL_Init(SDL_INIT_VIDEO);
}

Lib::~Lib()
{
	SDL_Quit();
}

bool
Lib::poll_event(SDL_Event* event)
{
	return SDL_PollEvent(event) > 0;
}
}