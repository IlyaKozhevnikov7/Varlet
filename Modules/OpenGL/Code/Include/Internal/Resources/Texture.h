#pragma once

#include "OpenGLCore.h"
#include "glad/glad.h"

enum class TextureType : uint8_t;

namespace Varlet::Graphics
{
	struct TextureCreateInfo;

	namespace OpenGL
	{
		struct Texture final
		{
		public:

			GLuint id;
			GLenum type;

		public:

			Texture() = default;

			Texture(GLuint id, GLenum type);

			Texture(const TextureCreateInfo& createInfo);

			~Texture();

			Texture& operator=(Texture&& other) noexcept;
		};

		extern GLuint CreateTexture(GLsizei width, GLsizei height, GLint format, GLenum textureType, const void* data, bool generateMipmap, GLint wrapType, GLint filter);
	}
}
