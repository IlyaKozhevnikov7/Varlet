#include "Rendering/Texture.h"
#include "Rendering/RendererAPI.h"

#include "stb/stb_image.h"

namespace Varlet
{
	std::unordered_map<std::string, Texture*> Texture::_loaded;

	Texture* Texture::Load(const char* path, const bool& mipmap, const bool& flipUV, const WrapType& wrapType, const FilterType& filter)
	{
		if (_loaded.contains(path) == false)
		{
			int32_t width, height;
			TextureFormat format;

			void* data = Load(path, flipUV, format, width, height);

			LoadableTextureConfiguration configuration;
			configuration.width = width;
			configuration.height = height;
			configuration.format = format;
			configuration.data = data;
			configuration.mipmap = mipmap;
			configuration.wrapType = wrapType;
			configuration.filter = filter;

			_loaded[path] = Varlet::RendererAPI::CreateTexture(configuration);

			stbi_image_free(data);
		}

		return _loaded[path];
	}

	int32_t Texture::GetWidth() const
	{
		return _width;
	}

	int32_t Texture::GetHeight() const
	{
		return _height;
	}

	TextureFormat Texture::GetFormat() const
	{
		return _format;
	}

	void* Texture::Load(const char* path, const bool& flipUV, TextureFormat& format, int32_t& width, int32_t& height)
	{
		stbi_set_flip_vertically_on_load(flipUV);

		int32_t nrChannels;
		void* data = stbi_load(path, &width, &height, &nrChannels, 0);

		format = nrChannels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;

		if (data == nullptr)
		{
			VARLET_LOG(LevelType::Error, "Failed to load texture " + *path);
			stbi_image_free(data);
		}

		return data;
	}
}
