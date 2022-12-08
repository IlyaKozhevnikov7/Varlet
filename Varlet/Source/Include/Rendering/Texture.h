#pragma once

#include "VarletCore.h"

enum class WrapType : uint8_t
{
	Repeat = 0,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder
};

enum class FilterType : uint8_t
{
	Linear = 0,
	Nearers
};

enum class TextureFormat : uint8_t
{
	RGB = 0,
	RGBA,
	RGB111110F,
	RGB16F,
	RGBA16F,
	RGB32F,
	RGBA32F
};

namespace Varlet
{
	struct LoadableTextureConfiguration final
	{
		int32_t width;
		int32_t height;
		TextureFormat format;
		const void* data;
		bool mipmap;
		WrapType wrapType;
		FilterType filter;
	};

	class CORE_API Texture
	{
	private:

		static std::unordered_map<std::string, Texture*> _loaded;

	protected:

		int32_t _width = 0;
		int32_t _height = 0;
		TextureFormat _format;

	protected:

		Texture() = default;

	public:

		static Texture* Load(const char* path, const bool& mipmap = true, const bool& flipUV = true, const WrapType& wrapType = WrapType::Repeat, const FilterType& filter = FilterType::Linear);

		virtual ~Texture() = default;

		int32_t GetWidth() const;

		int32_t GetHeight() const;

		TextureFormat GetFormat() const;

	private:

		static void* Load(const char* path, const bool& flipUV, TextureFormat& format, int32_t& width, int32_t& height);
	};
}
