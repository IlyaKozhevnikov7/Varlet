#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class OpenGLRendererAPI final : public IRendererAPI
	{
	public:

		Shader* CreateShader(const ShaderInitializer& initializer) const override;

		CameraCore* CreateCameraCore() const override;

		Framebuffer* CreateFrameBuffer(const int32_t& width, const int32_t& height) const override;

		Texture* CreateTexture(const TextureConfiguration& configuration) const override;

		Texture* LoadTexture(const LoadableTextureConfiguration& configuration) const override;

		UniformBuffer* CreateUniformBuffer(const int64_t& size) const override;

		VertexArray* CreateVertexArray(const VertexArrayData& data) const override;
	};
}
