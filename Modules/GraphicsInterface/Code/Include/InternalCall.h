#pragma once

#include "GraphicsBaseTypes.h"

namespace Varlet::Graphics
{
	using CreateShaderSignature = bool(*)(void* /* descriptor */, const ShaderCreateInfo& /* createInfo */);
	using CreateModelSignature = bool(*)(void* /* descriptor */, const ModelCreateInfo& /* createInfo */);
	using CreateTextureSignature = bool(*)(void* /* descriptor */, const TextureCreateInfo& /* createInfo */);
	using CreateUniformBufferSignature = bool(*)(void* /* descriptor */, const UniformBufferCreateInfo& /* createInfo */);
	using CreateRenderPipelineSignature = bool(*)(void* /* descriptor */, const RenderPipelineCreateInfo& /* createInfo */);

	using BeginRenderPipelineSignature = bool(*)(void* /* descriptor */);
	using EndRenderPipelineSignature = bool(*)(void* /* descriptor */);
	using NextPassSignature = void(*)();
	using BindShaderSignature = void(*)(void* /* shaderDescriptor */, void* /* pipelineType */, const void* /* uniformBuffer */);
	using DrawSignature = void(*)(void* /* descriptor */);

	using GetNativeTextureSignature = const void*(*)(void* /* descriptor */);
	using GetShaderUniformInfosSignature = const std::vector<UniformInfo>*(*)(void* /* descriptor */);
	using UpdateUniformBufferDataSignature = bool(*)(void* /* descriptor */, void* /* data */, uint32_t /* size */, uint32_t /* offset */);

	using DrawVerticesSignature = void(*)(uint32_t /* vertexCount */);

	struct GRAPHICSINTERFACE_API InternalCall final
	{
		// =============== Create Functions ===============

		static CreateShaderSignature CreateShader;

		static CreateTextureSignature CreateTexture;

		static CreateModelSignature CreateModel;

		static CreateUniformBufferSignature CreateUniformBuffer;

		static CreateRenderPipelineSignature CreateRenderPipeline;

		// =============== Render Functions ===============

		static BeginRenderPipelineSignature BeginRenderPipeline;

		static EndRenderPipelineSignature EndRenderPipeline;

		static NextPassSignature NextPass;

		static BindShaderSignature BindShader;

		static DrawSignature Draw;

		static DrawVerticesSignature DrawVertices;

		// =============== Utils Functions ===============

		static GetNativeTextureSignature GetNativeTexture;

		static GetShaderUniformInfosSignature GetShaderUniformInfos;

		static UpdateUniformBufferDataSignature UpdateUniformBufferData;
	};
}	   