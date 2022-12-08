#include "OpenGL/Camera.h"
#include "OpenGL/Common.h"
#include "Logger.h"
#include <glad/glad.h>

namespace Varlet::OpenGL
{
	Framebuffer::Framebuffer() : 
		width(0), 
		height(0), 
		id(0), 
		renderbufferId(0)
	{
	}

	Framebuffer::Framebuffer(const int32_t& width, const int32_t& height) :
		width(width), 
		height(height)
	{
		Initialize(width, height, 1);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void Framebuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(const int32_t& width, const int32_t& height)
	{
		Free(*this);
		
		Initialize(width, height, attachments.size());
	}

	std::vector<uint8_t> Framebuffer::ReadPixels(const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment)
	{
		std::vector<uint8_t> pixels((width * height) * 4);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);

		glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		return pixels;
	}

	void Framebuffer::Free(Framebuffer& framebuffer)
	{
		for (int32_t i = 0; i < framebuffer.attachments.size(); i++)
			glDeleteTextures(1, &framebuffer.attachments[i]);

		glDeleteFramebuffers(1, &framebuffer.id);
		glDeleteRenderbuffers(1, &framebuffer.renderbufferId);
	}

	void Framebuffer::Initialize(int32_t width, int32_t height, int32_t attachmentCount)
	{
		// TODO maybe pass attachment specification for each

		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		attachments.resize(attachmentCount);

		for (int32_t i = 0; i < attachmentCount; i++)
		{
			
			attachments[i] = CreateTexture(width, height, GL_RGBA); // GL_R11F_G11F_B10F
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, attachments[i], 0);
		}

		glGenRenderbuffers(1, &renderbufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			VARLET_LOG(LevelType::Error, "Failed to create framebuffer");
			return;
		}

		this->width = width;
		this->height = height;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Camera::Camera(int32_t width, int32_t height)
	{
		framebuffer = Framebuffer(width, height);
	}

	Camera::~Camera() 
	{
		Framebuffer::Free(framebuffer);
	}

	void Camera::Resize(int32_t width, int32_t height, const bool& dynamically)
	{
		if (dynamically && (width < framebuffer.width || height < framebuffer.height))
			return;

		framebuffer.Resize(width, height);
	}

	void Camera::Bind() const
	{
		framebuffer.Bind();
	}

	std::vector<uint8_t> Camera::ReadPixels(const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment)
	{
		return framebuffer.ReadPixels(x, y, width, height, attachment);
	}
}
