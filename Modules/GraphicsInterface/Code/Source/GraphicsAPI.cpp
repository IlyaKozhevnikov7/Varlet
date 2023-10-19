#include "GraphicsAPI.h"
#include "InternalCall.h"

namespace Varlet::Graphics
{
	bool CreateShader(void* descriptor, const ShaderCreateInfo& createInfo)
	{
		return InternalCall::CreateShader(descriptor, createInfo);
	}

	bool CreateTexture(void* descriptor, const TextureCreateInfo& createInfo)
	{
		return InternalCall::CreateTexture(descriptor, createInfo);
	}

	bool CreateModel(void* descriptor, const ModelCreateInfo& createInfo)
	{
		return InternalCall::CreateModel(descriptor, createInfo);
	}

	bool CreateUniformBuffer(void* descriptor, const UniformBufferCreateInfo& createInfo)
	{
		return InternalCall::CreateUniformBuffer(descriptor, createInfo);
	}

	bool CreateRenderPipeline(void* descriptor, const RenderPipelineCreateInfo& createInfo)
	{
		return InternalCall::CreateRenderPipeline(descriptor, createInfo);
	}

	void BindShader(void* descriptor, void* pipelineType, const void* uniformBuffer)
	{
		return InternalCall::BindShader(descriptor, pipelineType, uniformBuffer);
	}

	const void* GetNativeTexture(void* descriptor)
	{
		return InternalCall::GetNativeTexture(descriptor);
	}

	const std::vector<UniformInfo>* GetShaderUniformInfos(void* descriptor)
	{
		return InternalCall::GetShaderUniformInfos(descriptor);
	}

	bool UpdateUniformBufferData(void* descriptor, void* data, uint32_t size, uint32_t offset)
	{
		return InternalCall::UpdateUniformBufferData(descriptor, data, size, offset);
	}
}
