#include "OpenGLFramebuffer.h"
#include "OpenGLTexture.h"
#include "glad/glad.h"

namespace Varlet
{
	OpenGLFramebuffer::OpenGLFramebuffer(FramebufferConfiguration& configuration)
	{
		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		const int32_t texturesAmount = configuration.textureConfigurations.size();
		_textures = std::vector<OpenGLTexture*>(texturesAmount);

		for (int32_t i = 0; i < texturesAmount; i++)
		{
			configuration.textureConfigurations[i].width = configuration.width;
			configuration.textureConfigurations[i].height = configuration.height;

			const auto texture = dynamic_cast<OpenGLTexture*>(RendererAPI::CreateTexture(configuration.textureConfigurations[i]));
			_textures[i] = texture;

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->GetId(), 0);
		}

		glGenRenderbuffers(1, &_renderbufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, configuration.width, configuration.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderbufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			VARLET_LOG(LevelType::Error, "Framebuffer is not created");
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &_id);
		glDeleteRenderbuffers(1, &_renderbufferId);
	}

	const Texture* OpenGLFramebuffer::GetTexture(const int32_t& attachment) const
	{
		return _textures[attachment];
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	void OpenGLFramebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void* OpenGLFramebuffer::ReadPixels(const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment) const
	{
		uint8_t data[3];

		glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);

		glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		return data;
	}
}
