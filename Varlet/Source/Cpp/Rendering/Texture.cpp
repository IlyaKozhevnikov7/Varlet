#include "Texture.h"

namespace Varlet
{
	const uint32_t& Texture::GetId() const
	{
		return _id;
	}

	void Texture::GetResolution(int32_t* width, int32_t* height) const
	{
		*width = _width;
		*height = _height;
	}
}
