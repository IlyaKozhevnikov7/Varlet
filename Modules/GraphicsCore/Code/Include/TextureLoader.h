#pragma once

#include <stdint.h>

enum class WrapType : uint8_t;
enum class FilterType : uint8_t;
enum class TextureFormat : uint8_t;

namespace Varlet
{
	class TextureLoader final
	{
	public:

		static void* Load(const wchar_t* path, bool flipUV, TextureFormat& format, int32_t& width, int32_t& height);

		static void Free(void* data);

	private:

		static TextureFormat ConvertStbiFormatToVarletFormat(int32_t format);
	};
}
