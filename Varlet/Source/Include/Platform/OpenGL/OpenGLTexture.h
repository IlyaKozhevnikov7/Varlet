#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class CORE_API OpenGLTexture : public Texture
	{
	public:

		OpenGLTexture(const TextureConfiguration& configuration);

		OpenGLTexture(const LoadableTextureConfiguration& configuration);

		~OpenGLTexture() override;

		void Activate(const uint32_t& unit) const override;

	private:

		void Load(const LoadableTextureConfiguration& configuration) const;

		void Configurate(const TextureConfiguration& configuration) const;
	};
}
