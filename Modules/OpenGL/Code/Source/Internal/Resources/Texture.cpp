#include "Internal/Resources/Texture.h"
#include "Internal/Utils.h"

#include "GraphicsBaseTypes.h"

namespace Varlet::Graphics::OpenGL
{
	Texture::Texture(GLuint id, GLenum type) :
		id(id),
		type(type)
	{
	}

	Texture::Texture(const TextureCreateInfo& createInfo)
	{
		type = Utils::ConvertToGLTextureType(createInfo.type);

		id = OpenGL::CreateTexture(
			createInfo.width,
			createInfo.height,
			Utils::ConvertToGLFormat(createInfo.format),
			type,
			createInfo.data,
			createInfo.generateMipmap,
			Utils::ConvertToGLWrapType(createInfo.wrapType),
			Utils::ConvertToGLFilterType(createInfo.filterType));
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		id = other.id;
		other.id = 0;

		return *this;
	}

	GLuint CreateTexture(GLsizei width, GLsizei height, GLint format, GLenum textureType, const void* data, bool generateMipmap, GLint wrapType, GLint filter)
	{
		const GLenum nativeFormat = Utils::GetFormat(format);
		const GLenum nativeType = Utils::GetType(format);

		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(textureType, id);

		if (textureType == GL_TEXTURE_2D)
		{
			glTexImage2D(textureType, 0, format, width, height, 0, nativeFormat, nativeType, data);
		}
		else if (textureType == GL_TEXTURE_CUBE_MAP)
		{
			for (int32_t i = 0; i < 6; i++)
			{
				void* sourceData = (char*)data + width * height * i;
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, nativeFormat, nativeType, sourceData);
			}
		}

		if (generateMipmap)
			glGenerateMipmap(textureType);

		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapType);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapType);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_R, wrapType);
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // for optimization
		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filter);

		glBindTexture(textureType, 0);

		return id;
	}
}
