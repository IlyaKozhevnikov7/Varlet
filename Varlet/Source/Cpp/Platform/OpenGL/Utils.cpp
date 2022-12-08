#include "OpenGL/Utils.h"
#include "OpenGL/DescriptorPool.h"
#include <glad/glad.h>
#include "OpenGL/Texture.h"
#include "Material.h"

namespace Varlet::OpenGL
{
	int32_t Utils::ConvertToGLFormat(const TextureFormat& format)
	{
		switch (format)
		{
		case TextureFormat::RGB:		return GL_RGB;
		case TextureFormat::RGBA:		return GL_RGBA;
		case TextureFormat::RGB111110F: return GL_R11F_G11F_B10F;
		case TextureFormat::RGB16F:		return GL_RGB16F;
		case TextureFormat::RGBA16F:	return GL_RGBA16F;
		case TextureFormat::RGB32F:		return GL_RGB32F;
		case TextureFormat::RGBA32F:	return GL_RGBA16F;
		}
	}

	int32_t Utils::GetFormat(const int32_t& format)
	{
		switch (format)
		{
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

	int32_t Utils::GetType(const int32_t& format)
	{
		switch (format)
		{
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

	int32_t Utils::ConvertToGLWrapType(const WrapType& wrapType)
	{
		switch (wrapType)
		{
		case WrapType::Repeat:			return GL_REPEAT;
		case WrapType::MirroredRepeat:	return GL_MIRRORED_REPEAT;
		case WrapType::ClampToEdge:		return GL_CLAMP_TO_EDGE;
		case WrapType::ClampToBorder:	return GL_CLAMP_TO_BORDER;
		}
	}

	int32_t Utils::ConvertToGLFilterType(const FilterType& filterType)
	{
		switch (filterType)
		{
		case FilterType::Linear:	return GL_LINEAR;
		case FilterType::Nearers:	return GL_NEAREST;
		}
	}

	int32_t Utils::ConvertToGLFunc(const StensilFunction& func)
	{
		switch (func)
		{
		case StensilFunction::Never:			return GL_NEVER;
		case StensilFunction::Less:				return GL_LESS;
		case StensilFunction::LessOrEqual:		return GL_LEQUAL;
		case StensilFunction::Greater:			return GL_GREATER;
		case StensilFunction::GreaterOrEqual:	return GL_GEQUAL;
		case StensilFunction::Equal:			return GL_EQUAL;
		case StensilFunction::NotEqual:			return GL_NOTEQUAL;
		case StensilFunction::Always:			return GL_ALWAYS;
		}
	}

	int32_t Utils::ConvertToGLOp(const StensilOp& op)
	{
		switch (op)
		{
		case StensilOp::Keep:		return GL_KEEP;
		case StensilOp::Zero:		return GL_ZERO;
		case StensilOp::Replace:	return GL_REPLACE;
		case StensilOp::Incr:		return GL_INCR;
		case StensilOp::IncrWrap:	return GL_INCR_WRAP;
		case StensilOp::Decr:		return GL_DECR;
		case StensilOp::DecrWrap:	return GL_DECR_WRAP;
		case StensilOp::Invert:		return GL_INVERT;
		}
	}

	Varlet::Texture* Utils::GLTextureToVarletTexture(const uint32_t& textureId)
	{
		return new OpenGL::Texture(textureId);
	}
}
