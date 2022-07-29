#pragma once

#include <array>
#include <memory>

#include <SDLxx/Sprite.hxx>
#include <PieceType.hxx>

namespace SDLxx
{
	class Renderer;
	class Texture;
}

struct Resources
{
	std::array<SDLxx::Sprite, PieceCount> blackPiecesSprites;
	std::array<SDLxx::Sprite, PieceCount> whitePiecesSprites;
	std::shared_ptr<SDLxx::Texture> marbleAndStoneBoardTexture;
	std::shared_ptr<SDLxx::Texture> blackPiecesTexture;
	std::shared_ptr<SDLxx::Texture> whitePiecesTexture;

	int pieceCenterOffsetX;
	int pieceCenterOffsetY;

	static Resources Load(SDLxx::Renderer &renderer);
};
