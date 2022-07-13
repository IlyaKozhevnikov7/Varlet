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

		OpenGLFramebuffer(const int32_t& width, const int32_t& height);

		~OpenGLFramebuffer() override;

		const Texture* GetTexture() const override;

		void Bind() override;

		void UnBind() override;
	};
}
