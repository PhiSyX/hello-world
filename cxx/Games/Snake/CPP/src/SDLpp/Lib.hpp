#pragma once

union SDL_Event;

namespace SDLpp {
class Lib
{
  public:
	Lib();
	Lib(const Lib&) = delete;
	Lib(Lib&&) = delete;
	~Lib();

	Lib& operator=(const Lib&) = delete;
	Lib& operator=(Lib&&) = delete;

	bool poll_event(SDL_Event* event);
};
}
