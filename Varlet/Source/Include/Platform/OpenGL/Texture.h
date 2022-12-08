#pragma once

#include "Rendering/Texture.h"

namespace Varlet::OpenGL
{
	class Texture final : public Varlet::Texture
	{
	public:

		uint32_t id;

	public:

		Texture(const uint32_t& textureId);

		Texture(const LoadableTextureConfiguration& configuration);

		~Texture() override;
	};
}
