#pragma once

union SDL_Event;

namespace SDLxx
{
	class Lib
	{
	public:
		Lib();
		Lib(const Lib &) = delete;
		Lib(Lib &&) = delete;
		~Lib();

		bool PollEvent(SDL_Event *event);

		Lib &operator=(const Lib &) = delete;
		Lib &operator=(Lib &&) = delete;
	};
}
