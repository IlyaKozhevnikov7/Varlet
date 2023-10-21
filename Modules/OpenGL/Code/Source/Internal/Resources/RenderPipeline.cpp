#include "Internal/Resources/RenderPipeline.h"
#include "Internal/Resources/Texture.h"
#include "Internal/Resources.h"
#include "Internal/Utils.h"

namespace Varlet::Graphics::OpenGL
{
	RenderPipeline::RenderPipeline(const RenderPipelineCreateInfo& createInfo)
	{
		glGenFramebuffers(1, &framebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

		renderPasses = createInfo.renderPasses;

		for (auto& createInfo : createInfo.renderTargetCreateInfos)
			CreateAttachment(createInfo);

		drawAttachemtentCount = 0;
		for (auto& pass : createInfo.renderPasses)
			drawAttachemtentCount = std::max(drawAttachemtentCount, (GLsizei)pass.attachments.size());

		attachemtIds.reserve(drawAttachemtentCount);

		for (GLsizei i = 0; i < drawAttachemtentCount; i++)
			attachemtIds.push_back(GL_COLOR_ATTACHMENT0 + i);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// TODO: Check valid passes (attachment resolutions ...)
	}

	RenderPipeline::~RenderPipeline()
	{
		glDeleteFramebuffers(1, &framebufferId);
		glDeleteRenderbuffers(1, &depthStencilAttachmentId);
	}

	RenderPipeline& RenderPipeline::operator=(RenderPipeline&& other) noexcept
	{
		framebufferId = other.framebufferId;
		depthStencilAttachmentId = other.depthStencilAttachmentId;
		drawAttachemtentCount = other.drawAttachemtentCount;

		other.framebufferId = 0;
		other.depthStencilAttachmentId = 0;

		renderTargets = std::move(other.renderTargets);
		renderPasses = std::move(other.renderPasses);
		attachemtIds = std::move(other.attachemtIds);

		return *this;
	}

	void RenderPipeline::Begin()
	{
		ClearBuffers();

		currentPass = 0;
		SetupPass(currentPass);
	}

	void RenderPipeline::NextPass()
	{
		++currentPass;
		SetupPass(currentPass);
	}

	void RenderPipeline::SetupPass(uint32_t pass)
	{
		auto& currentPass = renderPasses[pass];
		const auto currentAttachmentCount = currentPass.attachments.size();

		for (GLenum i = 0; i < currentAttachmentCount; i++)
		{
			const auto& attachmentInfo = renderTargets[currentPass.attachments[i]];

			glBindTexture(GL_TEXTURE_2D, attachmentInfo.id);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, attachmentInfo.id, 0);

			if (i == 0)
				glViewport(0, 0, attachmentInfo.width, attachmentInfo.height);
		}

		if (pass == 0 || drawAttachemtentCount != currentAttachmentCount)
		{
			drawAttachemtentCount = currentAttachmentCount;
			glDrawBuffers(drawAttachemtentCount, attachemtIds.data());
		}
	}

	void RenderPipeline::ClearBuffers()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferId);

		for (auto& target : renderTargets)
		{
			glBindTexture(GL_TEXTURE_2D, target.second.id);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target.second.id, 0);

			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
	}

	void RenderPipeline::CreateAttachment(const RenderTargetCreateInfo& createInfo)
	{
		if (renderTargets.contains(createInfo.descriptor))
			return;

		const GLint glFormat = Utils::ConvertToGLFormat(createInfo.format);
		const GLenum format = Utils::GetFormat(glFormat);
		const GLenum type = Utils::GetType(glFormat);
		const GLint filter = Utils::ConvertToGLFilterType(createInfo.filter);

		if (glFormat == GL_DEPTH24_STENCIL8)
		{
			glGenRenderbuffers(1, &depthStencilAttachmentId);
			glBindRenderbuffer(GL_RENDERBUFFER, depthStencilAttachmentId);
			glRenderbufferStorage(GL_RENDERBUFFER, glFormat, createInfo.width, createInfo.height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilAttachmentId);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		else
		{
			const GLuint textureId = CreateTexture(createInfo.width, createInfo.height, glFormat, GL_TEXTURE_2D, nullptr, false, GL_CLAMP_TO_EDGE, filter);
			renderTargets[createInfo.descriptor] = { textureId, createInfo.width, createInfo.height, format, type };

			Resources::RegisterTexture(createInfo.descriptor, textureId, type);
		}
	}
}
