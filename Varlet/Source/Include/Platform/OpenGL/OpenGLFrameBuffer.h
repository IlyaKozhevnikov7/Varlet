#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class CORE_API OpenGLFramebuffer final : public Framebuffer
	{
	private:

		uint32_t _id;
		uint32_t textureId;
		uint32_t renderbufferId;

	public:

		OpenGLFramebuffer();

		~OpenGLFramebuffer() override;

		const uint32_t& GetTexture() const override;

		void Bind() override;

		void UnBind() override;
	};
}
