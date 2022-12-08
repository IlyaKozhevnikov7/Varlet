#pragma once 

#include <vector>

namespace Varlet::OpenGL
{
	extern uint32_t CreateTexture(const int32_t& width, const int32_t& height, const int32_t& format);

	extern uint32_t CreateTexture(const int32_t& width, const int32_t& height, const int32_t& format, const uint32_t& mipmapCount, std::vector<uint32_t>& mipmaps);

	extern uint32_t CreateTexture(const int32_t& width, const int32_t& height, const int32_t& format, const void* data, const bool& mipmap = false, const int32_t& wrapType = 0x2901 /* REPEAT */, const int32_t& filter = 0x2601 /* Linear */);
}
