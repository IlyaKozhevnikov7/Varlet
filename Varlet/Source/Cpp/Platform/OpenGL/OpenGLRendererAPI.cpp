#include "OpenGLRendererAPI.h"
#include "OpenGLShader.h"
#include "OpenGLCameraCore.h"
#include "OpenGLUniformBuffer.h"
#include "OpenGLFramebuffer.h"

namespace Varlet
{
	Shader* OpenGLRendererAPI::CreateShader(const ShaderInitializer* initializer) const
	{
		return new OpenGLShader(initializer);
	}

	CameraCore* OpenGLRendererAPI::CreateCameraCore() const
	{
		return new OpenGLCameraCore();
	}

	Framebuffer* OpenGLRendererAPI::CreateFrameBuffer() const
	{
		return new OpenGLFramebuffer();
	}

	UniformBuffer* OpenGLRendererAPI::CreateUniformBuffer(const int64_t& size) const
	{
		return new OpenGLUniformBuffer(size);
	}
}
