#include "Internal/Utils.h"
#include "GraphicsBaseTypes.h"

namespace Varlet::Graphics::OpenGL
{
	GLint Utils::ConvertToGLFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RED:		return GL_R8;
		case TextureFormat::RG:			return GL_RG;
		case TextureFormat::RGB:		return GL_RGB;
		case TextureFormat::RGBA:		return GL_RGBA;
		case TextureFormat::RGB111110F: return GL_R11F_G11F_B10F;
		case TextureFormat::RGB16F:		return GL_RGB16F;
		case TextureFormat::RGBA16F:	return GL_RGBA16F;
		case TextureFormat::RGB32F:		return GL_RGB32F;
		case TextureFormat::RGBA32F:	return GL_RGBA32F;
		case TextureFormat::D24S8:		return GL_DEPTH24_STENCIL8;
		}
	}

	GLenum Utils::GetFormat(GLint /*internal*/ format)
	{
		switch (format)
		{
		case GL_R8:
			return GL_RED;

		case GL_RG:
			return GL_RG;

		case GL_RGB:
		case GL_R11F_G11F_B10F:
		case GL_RGB16F:
		case GL_RGB32F:
			return GL_RGB;

		case GL_RGBA:
		case GL_RGBA16F:
		case GL_RGBA32F:
			return GL_RGBA;

		default:
			return GL_RGB;
		}
	}

	GLenum Utils::GetType(GLint format)
	{
		switch (format)
		{
		case GL_R8:
		case GL_RG:
		case GL_RGB:
		case GL_RGBA:
			return GL_UNSIGNED_BYTE;

		case GL_R11F_G11F_B10F:
		case GL_RGB16F:
		case GL_RGBA16F:
		case GL_RGB32F:
		case GL_RGBA32F:
			return GL_FLOAT;

		default:
			return GL_UNSIGNED_BYTE;
		}
	}

	GLint Utils::ConvertToGLWrapType(WrapType wrapType)
	{
		switch (wrapType)
		{
		case WrapType::Repeat:			return GL_REPEAT;
		case WrapType::MirroredRepeat:	return GL_MIRRORED_REPEAT;
		case WrapType::ClampToEdge:		return GL_CLAMP_TO_EDGE;
		case WrapType::ClampToBorder:	return GL_CLAMP_TO_BORDER;
		}
	}

	GLint Utils::ConvertToGLFilterType(FilterType filterType)
	{
		switch (filterType)
		{
		case FilterType::Linear:	return GL_LINEAR;
		case FilterType::Nearers:	return GL_NEAREST;
		}
	}

	GLenum Utils::ConvertToGLTextureType(TextureType type)
	{
		switch (type)
		{
		case TextureType::Texture1D:	return GL_TEXTURE_1D;
		case TextureType::Texture2D:	return GL_TEXTURE_2D;
		case TextureType::Cubmap:		return GL_TEXTURE_CUBE_MAP;
		}
	}
}
