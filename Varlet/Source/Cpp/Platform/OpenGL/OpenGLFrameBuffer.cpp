#include "OpenGLFramebuffer.h"
#include "OpenGLTexture.h"
#include "glad/glad.h"

namespace Varlet
{
	OpenGLFramebuffer::OpenGLFramebuffer(FramebufferConfiguration& configuration)
	{
		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		configuration.textureConfiguration.width = configuration.width;
		configuration.textureConfiguration.height = configuration.height;
		_texture = dynamic_cast<OpenGLTexture*>(RendererAPI::CreateTexture(configuration.textureConfiguration));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture->GetId(), 0);

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
		delete _texture;

		glDeleteFramebuffers(1, &_id);
		glDeleteRenderbuffers(1, &_renderbufferId);
	}

	const Texture* OpenGLFramebuffer::GetTexture() const
	{
		return _texture;
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	void OpenGLFramebuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void* OpenGLFramebuffer::ReadPixels(const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height) const
	{
		uint8_t data[3];

		glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		return data;
	}
}
