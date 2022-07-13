#include "OpenGLRendererAPI.h"
#include "OpenGLShader.h"
#include "OpenGLCameraCore.h"
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

	CameraCore* OpenGLRendererAPI::CreateCameraCore() const
	{
		return new OpenGLCameraCore();
	}

	Framebuffer* OpenGLRendererAPI::CreateFrameBuffer(const int32_t& width, const int32_t& height) const
	{
		return new OpenGLFramebuffer(width, height);
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
