#pragma once

#include <SDLxx/Texture.hxx>

namespace SDLxx
{
	class Renderer;
}

struct Resources
{
	SDLxx::Texture marbleAndStoneBoard;

	static Resources Load(SDLxx::Renderer &renderer);
};
