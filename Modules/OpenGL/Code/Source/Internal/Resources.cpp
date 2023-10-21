#include "Internal/Resources.h"
#include "Internal/Resources/Shader.h"
#include "Internal/Resources/Texture.h"
#include "Internal/Resources/Model.h"
#include "Internal/Resources/UniformBuffer.h"
#include "Internal/Resources/RenderPipeline.h"

#include "GraphicsBaseTypes.h"

namespace Varlet::Graphics::OpenGL
{
	std::unordered_map<void*, ShaderInfo> Resources::_loadedShaderInfos;

	std::unordered_map<void*, std::unordered_map<void*, Shader>> Resources::_shaders;
	std::unordered_map<void*, Texture> Resources::_textures;
	std::unordered_map<void*, Model> Resources::_models;
	std::unordered_map<void*, UniformBuffer> Resources::_uniformBuffers;
	std::unordered_map<void*, RenderPipeline> Resources::_pipelines;

	bool Resources::CreateShader(void* descriptor, const ShaderCreateInfo& createInfo)
	{
		assert(descriptor);
		assert(createInfo.source);

		auto createResult = Shader::Create(*createInfo.source);

		if (createResult.isSuccess == false)
			return false;

		_shaders[descriptor][createInfo.pipelineType] = std::move(createResult.shader);

		if (_loadedShaderInfos.contains(descriptor) == false)
			_loadedShaderInfos[descriptor] = { *createInfo.source, std::move(createResult.uniformInfos), std::move(createResult.uniformLocations) };

		return true;
	}

	bool Resources::CreateTexture(void* descriptor, const TextureCreateInfo& createInfo)
	{
		assert(descriptor);

		if (_textures.contains(descriptor))
		{
			VT_LOG(Warning, "Texture (descriptor %i) already exist", descriptor);
			return false;
		}

		_textures[descriptor] = Texture(createInfo);
		return true;
	}

	bool Resources::CreateModel(void* descriptor, const ModelCreateInfo& createInfo)
	{
		assert(descriptor);

		if (_models.contains(descriptor))
		{
			VT_LOG(Warning, "Model (descriptor %i) already exist", descriptor);
			return false;
		}

		_models[descriptor] = Model(createInfo);
		return true;
	}

	bool Resources::CreateUniformBuffer(void* descriptor, const UniformBufferCreateInfo& createInfo)
	{
		assert(descriptor);

		if (_uniformBuffers.contains(descriptor))
		{
			VT_LOG(Warning, "Uniform buffer (descriptor %i) already exist", descriptor);
			return false;
		}

		_uniformBuffers[descriptor] = UniformBuffer(createInfo);
		return true;
	}

	const void* Resources::GetNativeTexture(void* descriptor)
	{
		const auto texture = Resources::GetTexture(descriptor);

		return texture != nullptr
			? &texture->id
			: nullptr;
	}

	const std::vector<UniformInfo>* Resources::GetShaderUniformInfos(void* descriptor)
	{
		if (_loadedShaderInfos.contains(descriptor) == false)
			return nullptr;

		return &_loadedShaderInfos[descriptor].uniformInfos;
	}

	bool Resources::UpdateUniformBufferData(void* descriptor, void* data, uint32_t size, uint32_t offset)
	{
		if (_uniformBuffers.contains(descriptor) == false)
			return false;

		auto& uniformBuffer = _uniformBuffers[descriptor];
		uniformBuffer.UpdateData(data, size, offset);
	}

	bool Resources::CreateRenderPipeline(void* descriptor, const RenderPipelineCreateInfo& createInfo)
	{
		assert(descriptor);
		assert(createInfo.renderTargetCreateInfos.size() > 0);
		assert(createInfo.renderPasses.size() > 0);

		_pipelines[descriptor] = RenderPipeline(createInfo);

		return true;
	}

	void Resources::RegisterTexture(void* descriptor, GLuint id, GLenum type)
	{
		assert(descriptor);

		_textures[descriptor] = Texture(id, type);
	}

	RenderPipeline* Resources::GetRenderPipeline(void* descriptor)
	{
		assert(descriptor);

		return _pipelines.contains(descriptor)
			? &_pipelines[descriptor]
			: nullptr;
	}

	Model* Resources::GetModel(void* descriptor)
	{
		return _models.contains(descriptor)
			? &_models[descriptor]
			: nullptr;
	}

	Shader* Resources::GetShader(void* shaderDescriptor, void* pipelineDescriptor)
	{
		if (_shaders.contains(shaderDescriptor) == false)
			return nullptr;

		if (_shaders[shaderDescriptor].contains(pipelineDescriptor) == false)
			return nullptr;

		return &_shaders[shaderDescriptor][pipelineDescriptor];
	}

	Texture* Resources::GetTexture(void* descriptor)
	{
		return _textures.contains(descriptor)
			? &_textures[descriptor]
			: nullptr;
	}

	void Resources::AddShaderInfo(void* descriptor, const ShaderInfo& info)
	{
		assert(descriptor);
		assert(_loadedShaderInfos.contains(descriptor));
	}

	ShaderUniformInfosInternal Resources::GetShaderUniformInfosInternal(void* descriptor)
	{
		if (_loadedShaderInfos.contains(descriptor) == false)
			return { nullptr, nullptr, 0 };

		auto& info = _loadedShaderInfos[descriptor];
		return { &info.uniformInfos, &info.uniformLocations, info.firstTextureUnit };
	}
}
