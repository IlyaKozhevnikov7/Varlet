#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class CORE_API OpenGLFramebuffer final : public FrameBuffer
	{
	private:

		uint32_t _id;

	public:

		OpenGLFramebuffer();

		~OpenGLFramebuffer() override;

		void Bind() override;

		void UnBind() override;
	};
}
