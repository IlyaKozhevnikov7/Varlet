#pragma once

#include "GraphicsBaseTypes.h"

namespace Varlet::Graphics
{
	GRAPHICSINTERFACE_API extern bool CreateShader(void* descriptor, const ShaderCreateInfo& createInfo);

	GRAPHICSINTERFACE_API extern bool CreateTexture(void* descriptor, const TextureCreateInfo& createInfo);

	GRAPHICSINTERFACE_API extern bool CreateModel(void* descriptor, const ModelCreateInfo& createInfo);

	GRAPHICSINTERFACE_API extern bool CreateUniformBuffer(void* descriptor, const UniformBufferCreateInfo& createInfo);

	GRAPHICSINTERFACE_API extern bool CreateRenderPipeline(void* descriptor, const RenderPipelineCreateInfo& createInfo);

	GRAPHICSINTERFACE_API extern void BindShader(void* descriptor, void* pipelineType, const void* uniformBuffer);

	GRAPHICSINTERFACE_API extern const void* GetNativeTexture(void* descriptor);

	GRAPHICSINTERFACE_API extern const std::vector<UniformInfo>* GetShaderUniformInfos(void* descriptor);

	GRAPHICSINTERFACE_API extern bool UpdateUniformBufferData(void* descriptor, void* data, uint32_t size, uint32_t offset);
}
