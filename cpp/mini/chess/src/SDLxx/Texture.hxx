#pragma once

#include <cstdint>
#include <memory>
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
		struct ConstructToken
		{
		};

	public:
		Texture() = default;
		explicit Texture(SDL_Texture *handle, ConstructToken);
		Texture(const Texture &) = delete;
		Texture(Texture &&) noexcept = default;
		~Texture();
		Texture &operator=(const Texture &) = delete;
		Texture &operator=(Texture &&) noexcept = default;

		SDL_Rect GetRect() const;

		int Query(std::uint32_t *format, int *access, int *width, int *height) const;

		static std::shared_ptr<Texture> LoadFromFile(Renderer &renderer, const std::string &filepath);

	private:
		friend class Renderer;

		const SDL_Texture *GetHandle() const;

		MovablePtr<SDL_Texture> m_handle;
	};
}
