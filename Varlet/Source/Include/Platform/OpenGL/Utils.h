#pragma once

#include "VarletCore.h"

enum class WrapType : uint8_t;
enum class FilterType : uint8_t;
enum class TextureFormat : uint8_t;

namespace Varlet
{
	class Texture;
}

namespace Varlet::OpenGL
{
	class Utils final
	{
	public:

		static int32_t ConvertToGLFormat(const TextureFormat& format);

		static int32_t GetFormat(const int32_t& /*internal*/ format);

		static int32_t GetType(const int32_t& format);

		static int32_t ConvertToGLWrapType(const WrapType& wrapType);

		static int32_t ConvertToGLFilterType(const FilterType& filterType);

		static Varlet::Texture* GLTextureToVarletTexture(const uint32_t& textureId);
	};
}
