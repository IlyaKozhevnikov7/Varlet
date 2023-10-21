#pragma once

#include "OpenGLCore.h"

namespace Varlet::Graphics
{
	struct UniformInfo;

	struct ShaderCreateInfo;
	struct TextureCreateInfo;
	struct ModelCreateInfo;
	struct UniformBufferCreateInfo;
	struct RenderPipelineCreateInfo;

	namespace OpenGL
	{
		extern bool CreateShader(void* descriptor, const ShaderCreateInfo& createInfo);

		extern bool CreateTexture(void* descriptor, const TextureCreateInfo& createInfo);

		extern bool CreateModel(void* descriptor, const ModelCreateInfo& createInfo);

		extern bool CreateUniformBuffer(void* descriptor, const UniformBufferCreateInfo& createInfo);

		extern bool CreateRenderPipeline(void* descriptor, const RenderPipelineCreateInfo& createInfo);


		extern bool BeginRenderPipeline(void* descriptor);

		extern bool EndRenderPipeline(void* descriptor);

		extern void NextPass();

		extern void BindShader(void* shaderDescriptor, void* pipelineDescriptor, const void* uniformBuffer);

		extern void Draw(void* descriptor);

		extern void DrawVertices(uint32_t vertexCount);

		
		extern const void* GetNativeTexture(void* descriptor);

		extern const std::vector<UniformInfo>* GetShaderUniformInfos(void* descriptor);

		extern bool UpdateUniformBufferData(void* descriptor, void* data, uint32_t size, uint32_t offset);
	}
}