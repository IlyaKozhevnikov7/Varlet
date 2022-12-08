#include "OpenGL/Texture.h"
#include "OpenGL/Common.h"
#include "OpenGL/Utils.h"
#include "OpenGL/DescriptorPool.h"

#include <glad/glad.h>

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
			Utils::ConvertToGLFormat(configuration.format),
			configuration.data,
			configuration.mipmap,
			Utils::ConvertToGLWrapType(configuration.wrapType),
			Utils::ConvertToGLFilterType(configuration.filter));

		_width = configuration.width;
		_height = configuration.height;
	}

	Texture::~Texture()
	{
		DescriptorPool::Unregister(this);

		glDeleteTextures(1, &id);
	}
}
