#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class CORE_API OpenGLTexture : public Texture
	{
	public:

		OpenGLTexture(const TextureConfiguration& configuration);

		~OpenGLTexture() override;
	};
}
