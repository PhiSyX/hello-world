#include <cassert>
#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>
#include <SDLxx/Surface.hxx>

namespace SDLxx
{
	Surface::Surface(SDL_Surface *surface) : m_handle(surface)
	{
	}

	const SDL_Surface *Surface::GetHandle() const
	{
		return m_handle;
	}

	Surface::~Surface()
	{
		if (m_handle)
			SDL_FreeSurface(m_handle);
	}

	Surface Surface::LoadFromFile(const std::string &filepath)
	{
		SDL_Surface *surface = IMG_Load(filepath.c_str());
		if (!surface)
			throw std::runtime_error("failed to load " + filepath + ": " + IMG_GetError());

		return Surface(surface);
	}
}
