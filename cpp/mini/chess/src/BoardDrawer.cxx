#include <SDL.h>
#include <SDLxx/Renderer.hxx>

#include <BoardDrawer.hxx>
#include <Resources.hxx>

BoardDrawer::BoardDrawer(const Resources &resources, int width, int height) : m_resources(resources),
																			  m_height(height),
																			  m_width(width)
{
}

void BoardDrawer::Draw(SDLxx::Renderer &renderer, const Board &board)
{
	SDL_Rect textureRect = m_resources.marbleAndStoneBoard.GetRect();
	textureRect.x = m_width / 2 - textureRect.w / 2;
	textureRect.y = m_height / 2 - textureRect.h / 2;
	renderer.RenderCopy(m_resources.marbleAndStoneBoard, textureRect);
}
