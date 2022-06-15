#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class FrameBuffer
	{
	public:

		FrameBuffer() = default;

		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;
	};
}
