#pragma once

#include "VarletCore.h"

enum class TextureType : uint8_t
{
	Diffuse = 0,
	Specular

	// TODO add other types
};

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
public:

	std::string path;
	TextureType type;
	WrapType wrapType;
	FilterType filter;
	bool mipmap;
	bool flipUV;
};

namespace Varlet
{
	class CORE_API Texture
	{
	protected:

		uint32_t _id;
		TextureType _type;

	public:

		Texture(const TextureConfiguration& configuration);

		virtual ~Texture() = default;
	};
}
