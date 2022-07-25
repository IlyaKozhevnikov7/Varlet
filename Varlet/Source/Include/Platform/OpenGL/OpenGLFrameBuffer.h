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

		std::vector<OpenGLTexture*> _textures;

	public:

		OpenGLFramebuffer(FramebufferConfiguration& configuration);

		~OpenGLFramebuffer() override;

		const Texture* GetTexture(const int32_t& attachment) const override;

		void Bind() override;

		void UnBind() override;
	};
}
