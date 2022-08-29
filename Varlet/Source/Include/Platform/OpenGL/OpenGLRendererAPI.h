#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class OpenGLRendererAPI final : public IRendererAPI
	{
	private:

		mutable std::unordered_map<std::string, std::shared_ptr<Texture>> _textureBuffer;

	public:

		Shader* CreateShader(const ShaderInitializer& initializer) const override;

		Framebuffer* CreateFrameBuffer(FramebufferConfiguration& configuration) const override;

		Texture* CreateTexture(const TextureConfiguration& configuration) const override;

		std::shared_ptr<Texture> LoadTexture(const LoadableTextureConfiguration& configuration) const override;

		UniformBuffer* CreateUniformBuffer(const int64_t& size) const override;

		VertexArray* CreateVertexArray(const VertexArrayData& data) const override;

		void UnbindTexure(const uint32_t& unit) override;
	};
}
