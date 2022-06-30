#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace Varlet
{
	OpenGLTexture::OpenGLTexture(const TextureConfiguration& configuration) : Texture(configuration)
	{
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);

		int32_t wraptype;
		
		switch (configuration.wrapType)
		{
		case WrapType::Repeat:
			wraptype = GL_REPEAT;
			break;

		case WrapType::MirroredRepeat:
			wraptype = GL_MIRRORED_REPEAT;
			break;

		case WrapType::ClampToEdge:
			wraptype = GL_CLAMP_TO_EDGE;
			break;

		case WrapType::ClampToBorder:
			wraptype = GL_CLAMP_TO_BORDER;
			const float borderColor[] = { 1.f, 0.f, 1.f, 1.f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraptype);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wraptype);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wraptype);

		int32_t filter;

		switch (configuration.filter)
		{
		case FilterType::Linear:
			filter = GL_LINEAR;
			break;

		case FilterType::Nearers:
			filter = GL_NEAREST;
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		stbi_set_flip_vertically_on_load(configuration.flipUV);

		int32_t width, height, nrChannels;
		uint8_t* data = stbi_load(configuration.path.c_str(), &width, &height, &nrChannels, 0);

		const uint16_t format = nrChannels == 4 ? GL_RGBA : GL_RGB;

		if (data != nullptr)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			if (configuration.mipmap)
				glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}
		else
		{
			VARLET_LOG(LevelType::Error, "Failed to load texture " + configuration.path);
			return;
		}
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &_id);
	}
}
