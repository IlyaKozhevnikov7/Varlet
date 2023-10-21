#pragma once

#include "OpenGLCore.h"
#include "glad/glad.h"

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
		struct ShaderInfo;

		struct Shader;
		struct Texture;
		struct Model;
		struct UniformBuffer;
		struct RenderPipeline;

		struct ShaderUniformInfosInternal final
		{
			std::vector<UniformInfo>* uniformInfos;
			std::vector<GLint>* uniformLocations;
			GLenum firstTextureUnit;
		};

		class Resources final
		{
		private:

			static std::unordered_map<void*, ShaderInfo> _loadedShaderInfos;

			static std::unordered_map<void*, std::unordered_map<void*, Shader>> _shaders;
			static std::unordered_map<void*, Texture> _textures;
			static std::unordered_map<void*, Model> _models;
			static std::unordered_map<void*, UniformBuffer> _uniformBuffers;
			static std::unordered_map<void*, RenderPipeline> _pipelines;

		public:

			static bool CreateShader(void* descriptor, const ShaderCreateInfo& createInfo);

			static bool CreateTexture(void* descriptor, const TextureCreateInfo& createInfo);

			static bool CreateModel(void* descriptor, const ModelCreateInfo& createInfo);

			static bool CreateUniformBuffer(void* descriptor, const UniformBufferCreateInfo& createInfo);

			static bool CreateRenderPipeline(void* descriptor, const RenderPipelineCreateInfo& createInfo);



			static const void* GetNativeTexture(void* descriptor);

			static const std::vector<UniformInfo>* GetShaderUniformInfos(void* descriptor);

			static bool UpdateUniformBufferData(void* descriptor, void* data, uint32_t size, uint32_t offset);


			/*
			*	Internal functions
			*/
			
		public:

			static void RegisterTexture(void* descriptor, GLuint id, GLenum type);

			static RenderPipeline* GetRenderPipeline(void* descriptor);

			static Model* GetModel(void* descriptor);

			static Shader* GetShader(void* shaderDescriptor, void* pipelineDescriptor);

			static Texture* GetTexture(void* descriptor);

			static void AddShaderInfo(void* descriptor, const ShaderInfo& info);

			static ShaderUniformInfosInternal GetShaderUniformInfosInternal(void* descriptor);
		};
	}
}