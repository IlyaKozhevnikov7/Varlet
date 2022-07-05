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
	uint32_t width;
	uint32_t height;
	WrapType wrapType;
	FilterType filter;
	bool mipmap;
};

struct LoadableTextureConfiguration : public TextureConfiguration
{
	char* path;
	bool flipUV;
};

namespace Varlet
{
	class CORE_API Texture
	{
	protected:

		uint32_t _id;

	public:

		Texture() = default;

		virtual ~Texture() = default;

		virtual void Activate(const uint32_t& unit) const = 0;

		virtual const uint32_t& GetId() const;
	};
}
