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

struct TextureConfiguration
{
	int32_t width;
	int32_t height;
	WrapType wrapType;
	FilterType filter;
	bool mipmap;
};

struct LoadableTextureConfiguration : public TextureConfiguration
{
	const char* path;
	bool flipUV;
};

namespace Varlet
{
	class CORE_API Texture
	{
	protected:

		uint32_t _id;
		int32_t _width;
		int32_t _height;

	public:

		Texture() = default;

		virtual ~Texture() = default;

		virtual void Activate(const uint32_t& unit) const = 0;

		const uint32_t& GetId() const;

		void GetResolution(int32_t* width, int32_t* height) const;
	};
}
