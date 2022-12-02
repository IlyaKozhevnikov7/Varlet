#include "OpenGL/Texture.h"
#include "OpenGL/Common.h"
#include "OpenGL/Utils.h"

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace Varlet::OpenGL
{
	Texture::Texture(const uint32_t& textureId)
	{
		id = textureId;
	}

	Texture::Texture(const Varlet::LoadableTextureConfiguration& configuration)
	{
		id = OpenGL::CreateTexture(
			configuration.width,
			configuration.height,
			OpenGL::Utils::ConvertToGLFormat(configuration.format),
			configuration.data,
			configuration.mipmap,
			OpenGL::Utils::ConvertToGLWrapType(configuration.wrapType),
			OpenGL::Utils::ConvertToGLFilterType(configuration.filter));

		_width = configuration.width;
		_height = configuration.height;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}

	void Texture::Activate(const uint32_t& unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, id);
	}
}
