#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class OpenGLTexture;

	class CORE_API OpenGLFramebuffer final : public Framebuffer
	{
	private:

		uint32_t _id;
		uint32_t renderbufferId;

		OpenGLTexture* texture;

	public:

		OpenGLFramebuffer();

		~OpenGLFramebuffer() override;

		const Texture* GetTexture() const override;

		void Bind() override;

		void UnBind() override;
	};
}
