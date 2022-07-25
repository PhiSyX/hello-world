#include <Resources.hxx>

Resources Resources::Load(SDLxx::Renderer &renderer)
{
	Resources resources;
	resources.marbleAndStoneBoard = SDLxx::Texture::LoadFromFile(renderer, "resources/Top Down/Boards/Full Boards/Marble and Stone 512x552.png");

	return resources;
}
