#pragma once

#include "GraphicsBaseTypes.h"
#include "glad/glad.h"

namespace Varlet::Graphics::OpenGL
{
	struct AttachmentInfo final
	{
		GLuint id;
		uint32_t width;
		uint32_t height;

		GLenum format;
		GLenum type;
	};

	struct RenderPipeline final
	{
	public:

		GLuint framebufferId = 0;
		GLuint depthStencilAttachmentId = 0;

		std::unordered_map<void*, AttachmentInfo> renderTargets;
		std::vector<RenderPassInfo> renderPasses;

		std::vector<GLenum> attachemtIds;
		GLsizei drawAttachemtentCount;

		uint32_t currentPass = 0;

	public:

		RenderPipeline() = default;

		RenderPipeline(const RenderPipelineCreateInfo& createInfo);

		~RenderPipeline();

		RenderPipeline& operator=(RenderPipeline&& other) noexcept;

		void Begin();

		void NextPass();

	private:

		void SetupPass(uint32_t pass);

		void ClearBuffers();

		void CreateAttachment(const RenderTargetCreateInfo& createInfo);
	};
}
