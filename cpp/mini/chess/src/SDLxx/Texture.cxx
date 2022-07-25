#include <cassert>
#include <stdexcept>
#include <utility>

#include <SDL.h>
#include <SDLxx/Texture.hxx>
#include <SDLxx/Renderer.hxx>
#include <SDLxx/Surface.hxx>

namespace SDLxx
{
	Texture::Texture(SDL_Texture *handle) : m_handle(handle)
	{
	}

	const SDL_Texture *Texture::GetHandle() const
	{
		return m_handle;
	}

	Texture::~Texture()
	{
		if (m_handle)
			SDL_DestroyTexture(m_handle);
	}

	SDL_Rect Texture::GetRect() const
	{
		int width, height;
		if (Query(nullptr, nullptr, &width, &height) != 0)
			throw std::runtime_error(std::string("failed to query texture dimensions: ") + SDL_GetError());

		return SDL_Rect{0, 0, width, height};
	}

	int Texture::Query(std::uint32_t *format, int *access, int *width, int *height) const
	{
		return SDL_QueryTexture(const_cast<SDL_Texture *>(m_handle.Get()), format, access, width, height); //< SDL is not const-correct
	}

	Texture Texture::LoadFromFile(Renderer &renderer, const std::string &filepath)
	{
		Surface surface = Surface::LoadFromFile(filepath);
		return renderer.CreateTextureFromSurface(surface);
	}
}
