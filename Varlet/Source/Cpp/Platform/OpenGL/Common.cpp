#include "Platform/OpenGL/Common.h"
#include "Platform/OpenGL/Utils.h"
#include <glad/glad.h>

namespace Varlet::OpenGL
{
	uint32_t CreateTexture(const int32_t& width, const int32_t& height, const int32_t& format)
	{
		const int32_t glInternalFormat = format;
		const int32_t glFormat = Utils::GetFormat(format);
		const int32_t glType = Utils::GetType(format);

		uint32_t id;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return id;
	}

	uint32_t CreateTexture(const int32_t& width, const int32_t& height, const int32_t& format, const uint32_t& mipmapCount, std::vector<uint32_t>& mipmaps)
	{
		const int32_t glInternalFormat = format;
		const int32_t glFormat = Utils::GetFormat(format);
		const int32_t glType = Utils::GetType(format);					

		uint32_t id = CreateTexture(width, height, format);

		int32_t mipWidth = width;
		int32_t mipheight = height;
		mipmaps.resize(mipmapCount);

		for (int32_t i = 0; i < mipmapCount; i++)
		{
			mipWidth /= 2;
			mipheight /= 2;

			uint32_t mipmapId;
			glGenTextures(1, &mipmapId);
			glBindTexture(GL_TEXTURE_2D, mipmapId);
			glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			mipmaps.push_back(mipmapId);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		return id;
	}

	uint32_t CreateTexture(const int32_t& width, const int32_t& height, const int32_t& format, const void* data, const bool& mipmap, const int32_t& wrapType, const int32_t& filter)
	{
		const int32_t nativeFormat = Utils::GetFormat(format);
		const int32_t nativeType = Utils::GetType(format);
	
		uint32_t id;
		
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, nativeFormat, nativeType, data);
	
		if (mipmap)
			glGenerateMipmap(GL_TEXTURE_2D);
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // for optimization
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	
		glBindTexture(GL_TEXTURE_2D, 0);

		return id;
	}
}
