#include "Internal/InterfaceFunctions.h"
#include "Internal/Resources.h"
#include "Internal/RenderStageCommand.h"

namespace Varlet::Graphics::OpenGL
{
	bool CreateShader(void* descriptor, const ShaderCreateInfo& createInfo)
	{
		return Resources::CreateShader(descriptor, createInfo);
	}

	bool CreateTexture(void* descriptor, const TextureCreateInfo& createInfo)
	{
		return Resources::CreateTexture(descriptor, createInfo);
	}

	bool CreateModel(void* descriptor, const ModelCreateInfo& createInfo)
	{
		return Resources::CreateModel(descriptor, createInfo);
	}

	bool CreateUniformBuffer(void* descriptor, const UniformBufferCreateInfo& createInfo)
	{
		return Resources::CreateUniformBuffer(descriptor, createInfo);
	}

	bool CreateRenderPipeline(void* descriptor, const RenderPipelineCreateInfo& createInfo)
	{
		return Resources::CreateRenderPipeline(descriptor, createInfo);
	}

	bool BeginRenderPipeline(void* descriptor)
	{
		return RenderStageCommand::BeginRenderPipeline(descriptor);
	}

	bool EndRenderPipeline(void* descriptor)
	{
		return RenderStageCommand::EndRenderPipeline(descriptor);
	}

	void NextPass()
	{
		RenderStageCommand::NextPass();
	}

	void BindShader(void* shaderDescriptor, void* pipelineDescriptor, const void* uniformBuffer)
	{
		RenderStageCommand::BindShader(shaderDescriptor, pipelineDescriptor, uniformBuffer);
	}

	void Draw(void* descriptor)
	{
		RenderStageCommand::Draw(descriptor);
	}

	void DrawVertices(uint32_t vertexCount)
	{
		RenderStageCommand::DrawVertices(vertexCount);
	}

	const void* GetNativeTexture(void* descriptor)
	{
		return Resources::GetNativeTexture(descriptor);
	}

	const std::vector<UniformInfo>* GetShaderUniformInfos(void* descriptor)
	{
		return Resources::GetShaderUniformInfos(descriptor);
	}

	bool UpdateUniformBufferData(void* descriptor, void* data, uint32_t size, uint32_t offset)
	{
		return Resources::UpdateUniformBufferData(descriptor, data, size, offset);
	}
}
