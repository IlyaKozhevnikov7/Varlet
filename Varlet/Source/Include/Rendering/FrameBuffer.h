#pragma once

#include "VarletCore.h"

struct TextureConfiguration;

struct FramebufferConfiguration final
{
	int32_t width;
	int32_t height;

	std::vector<TextureConfiguration> textureConfigurations;
};

namespace Varlet
{
	class Texture;

	class CORE_API Framebuffer
	{
	public:

		Framebuffer() = default;

		virtual ~Framebuffer() = default;

		virtual const Texture* GetTexture(const int32_t& attachment = 0) const = 0;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;
	};
}
