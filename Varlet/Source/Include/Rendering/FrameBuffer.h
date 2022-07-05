#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class CORE_API Framebuffer
	{
	public:

		Framebuffer() = default;

		virtual ~Framebuffer() = default;

		virtual const uint32_t& GetTexture() const = 0;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;
	};
}
