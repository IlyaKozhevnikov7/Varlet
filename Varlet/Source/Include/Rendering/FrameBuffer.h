#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class CORE_API Framebuffer
	{
	public:

		Framebuffer() = default;

		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;
	};
}
