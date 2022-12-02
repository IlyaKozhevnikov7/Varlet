#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class OpenGLRendererAPI final : public IRendererAPI
	{
	public:
		
		~OpenGLRendererAPI() override = default;

		Shader* CreateShader(const ShaderInitializer& initializer) const override;

		Texture* CreateTexture(const LoadableTextureConfiguration& configuration) const override;

		UniformBuffer* CreateUniformBuffer(const int64_t& size) const override;

		VertexArray* CreateVertexArray(const VertexArrayData& data) const override;

		Texture* GetTextureOf(Camera* camera) const override;

		void ChangeResolution(Camera* camera) const override;

		void GetFramebufferSize(Camera* camera, int32_t& width, int32_t& height) const override;

		std::vector<uint8_t> ReadRenderTexturePixels(Camera* camera, const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment = 0) const override;

		const void* GetNativeRenderTexture(Camera* camera, const uint32_t& attachment = 0) const override;

		const void* GetNativeTexture(Texture* texture) const override;
	};
}
