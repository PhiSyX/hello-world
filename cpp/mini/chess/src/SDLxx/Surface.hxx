#pragma once

#include <cstdint>
#include <string>

#include <MovablePtr.hxx>

struct SDL_Surface;

namespace SDLxx
{
	class Surface
	{
	public:
		Surface() = default;
		Surface(const Surface &) = delete;
		Surface(Surface &&) noexcept = default;
		~Surface();

		Surface &operator=(const Surface &) = delete;
		Surface &operator=(Surface &&) noexcept = default;

		static Surface LoadFromFile(const std::string &filepath);

	private:
		friend class Renderer;

		explicit Surface(SDL_Surface *surface);

		const SDL_Surface *GetHandle() const;

		MovablePtr<SDL_Surface> m_handle;
	};
}
