#pragma once

#include <cstdint>
#include <string>

#include <MovablePtr.hxx>

struct SDL_Rect;
struct SDL_Texture;

namespace SDLxx
{
	class Renderer;
	class Surface;

	class Texture
	{
	public:
		Texture() = default;
		Texture(const Texture &) = delete;
		Texture(Texture &&) noexcept = default;
		~Texture();

		SDL_Rect GetRect() const;

		int Query(std::uint32_t *format, int *access, int *width, int *height) const;

		Texture &operator=(const Texture &) = delete;
		Texture &operator=(Texture &&) noexcept = default;

		static Texture LoadFromFile(Renderer &renderer, const std::string &filepath);

	private:
		friend class Renderer;

		explicit Texture(SDL_Texture *handle);

		const SDL_Texture *GetHandle() const;

		MovablePtr<SDL_Texture> m_handle;
	};
}
