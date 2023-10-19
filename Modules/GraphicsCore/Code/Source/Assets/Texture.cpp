#include "Assets/Texture.h"

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