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
		uint32_t _renderbufferId;
		OpenGLTexture* _texture;

	public:

		OpenGLFramebuffer(FramebufferConfiguration& configuration);

		~OpenGLFramebuffer() override;

		const Texture* GetTexture() const override;

		void Bind() override;

		void UnBind() override;

		void* ReadPixels(const int32_t& x, const int32_t& y, const int32_t& width = 1, const int32_t& height = 1) const override;
	};
}
