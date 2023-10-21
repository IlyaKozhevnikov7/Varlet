#include "TextureLoader.h"
#include "Assets/Texture.h"
#include "Platform/Current.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace Varlet
{
    void* TextureLoader::Load(const wchar_t* path, bool flipUV, TextureFormat& format, int32_t& width, int32_t& height)
    {
		stbi_set_flip_vertically_on_load(flipUV);
		
		auto validPath = Varlet::Core::Platform::ConvertFromWide(path);
		
		int32_t nrChannels;
		void* data = stbi_load(validPath.c_str(), &width, &height, &nrChannels, 0);
		
		format = ConvertStbiFormatToVarletFormat(nrChannels);
		
		if (data == nullptr)
			stbi_image_free(data);
		
		return data;
    }

	void TextureLoader::Free(void* data)
	{
		stbi_image_free(data);
	}

	TextureFormat TextureLoader::ConvertStbiFormatToVarletFormat(int32_t format)
	{
		switch (format)
		{
		case 1:	return TextureFormat::RED;
		case 2:	return TextureFormat::RG;
		case 3:	return TextureFormat::RGB;
		case 4:	return TextureFormat::RGBA;
		}
	}
}
