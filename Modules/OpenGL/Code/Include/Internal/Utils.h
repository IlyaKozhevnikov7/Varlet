#pragma once

#include "OpenGLCore.h"
#include "glad/glad.h"

enum class WrapType : uint8_t;
enum class FilterType : uint8_t;
enum class TextureFormat : uint8_t;
enum class TextureType : uint8_t;

namespace Varlet::Graphics::OpenGL
{
	class Utils final
	{
	public:

		static GLint ConvertToGLFormat(TextureFormat format);

		static GLenum GetFormat(GLint /*internal*/ format);

		static GLenum GetType(GLint format);

		static GLint ConvertToGLWrapType(WrapType wrapType);

		static GLint ConvertToGLFilterType(FilterType filterType);

		static GLenum ConvertToGLTextureType(TextureType type);
	};
}
