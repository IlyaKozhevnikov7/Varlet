#include "OpenGLRendererAPI.h"
#include "OpenGLShader.h"
#include "OpenGLUniformBuffer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLVertexArray.h"

namespace Varlet
{
	Shader* OpenGLRendererAPI::CreateShader(const ShaderInitializer& initializer) const
	{
		return new OpenGLShader(initializer);
	}

	Framebuffer* OpenGLRendererAPI::CreateFrameBuffer(FramebufferConfiguration& configuration) const
	{
		return new OpenGLFramebuffer(configuration);
	}

	Texture* OpenGLRendererAPI::CreateTexture(const TextureConfiguration& configuration) const
	{
		return new OpenGLTexture(configuration);
	}

	Texture* OpenGLRendererAPI::LoadTexture(const LoadableTextureConfiguration& configuration) const
	{
		return new OpenGLTexture(configuration);
	}

	UniformBuffer* OpenGLRendererAPI::CreateUniformBuffer(const int64_t& size) const
	{
		return new OpenGLUniformBuffer(size);
	}

	VertexArray* OpenGLRendererAPI::CreateVertexArray(const VertexArrayData& data) const
	{
		return new OpenGLVertexArray(data);
	}
}