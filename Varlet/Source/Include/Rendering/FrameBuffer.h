#pragma once

#include "VarletCore.h"
#include "Rendering/Texture.h"

struct FramebufferConfiguration final
{
	int32_t width;
	int32_t height;
	TextureConfiguration textureConfiguration;
};

namespace Varlet
{
	class Texture;

	class CORE_API Framebuffer
	{
	public:

		Framebuffer() = default;

		virtual ~Framebuffer() = default;

		virtual const Texture* GetTexture() const = 0;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		virtual void* ReadPixels(const int32_t& x, const int32_t& y, const int32_t& width = 1, const int32_t& height = 1) const = 0;
	};
}