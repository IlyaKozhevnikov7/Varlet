#pragma once

#include "VarletCore.h"

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
	};
}
