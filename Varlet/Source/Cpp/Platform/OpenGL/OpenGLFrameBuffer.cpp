#include "OpenGLFramebuffer.h"
#include "glad/glad.h"

namespace Varlet
{
	OpenGLFramebuffer::OpenGLFramebuffer()
	{
		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		// redo with renderer api
		{
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

			glGenRenderbuffers(1, &renderbufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, renderbufferId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 640, 480);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

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
		glDeleteTextures(1, &textureId);
		glDeleteRenderbuffers(1, &renderbufferId);
	}

	const uint32_t& OpenGLFramebuffer::GetTexture() const
	{
		return textureId;
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
