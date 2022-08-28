#include "OpenGLRendererAPI.h"
#include "OpenGLShader.h"
#include "OpenGLUniformBuffer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

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

	std::shared_ptr<Texture> OpenGLRendererAPI::LoadTexture(const LoadableTextureConfiguration& configuration) const
	{
		if (_textureBuffer.contains(configuration.path) == false)
			_textureBuffer[configuration.path] = std::make_shared<OpenGLTexture>(configuration);

		return _textureBuffer[configuration.path];
	}

	UniformBuffer* OpenGLRendererAPI::CreateUniformBuffer(const int64_t& size) const
	{
		return new OpenGLUniformBuffer(size);
	}

	VertexArray* OpenGLRendererAPI::CreateVertexArray(const VertexArrayData& data) const
	{
		return new OpenGLVertexArray(data);
	}

	void OpenGLRendererAPI::UnbindTexure(const int32_t& unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
