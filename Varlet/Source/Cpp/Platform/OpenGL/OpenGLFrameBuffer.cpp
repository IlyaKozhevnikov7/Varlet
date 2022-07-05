#include "OpenGLFramebuffer.h"
#include "OpenGLTexture.h"
#include "glad/glad.h"

namespace Varlet
{
	OpenGLFramebuffer::OpenGLFramebuffer()
	{
		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		TextureConfiguration configuration
		{
			640,
			480,
			WrapType::ClampToEdge,
			FilterType::Linear,
			false
		};

		texture = dynamic_cast<OpenGLTexture*>(RendererAPI::CreateTexture(configuration));

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->_id, 0);

		glGenRenderbuffers(1, &renderbufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, configuration.width, configuration.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferId);
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
		glDeleteTextures(1, &texture->_id);
		glDeleteRenderbuffers(1, &renderbufferId);

		delete texture;
	}

	const Texture* OpenGLFramebuffer::GetTexture() const
	{
		return texture;
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	void OpenGLFramebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
