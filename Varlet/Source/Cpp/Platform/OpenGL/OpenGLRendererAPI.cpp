#include "OpenGL/OpenGLRendererAPI.h"
#include "OpenGL/OpenGLUniformBuffer.h"
#include "OpenGL/OpenGLVertexArray.h"

// new
#include "OpenGL/ObjectMap.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Utils.h"

#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"

namespace Varlet
{
	Shader* OpenGLRendererAPI::CreateShader(const ShaderInitializer& initializer) const
	{
		const auto& shader = new OpenGL::Shader(initializer);
		OpenGL::ShaderCache::Add(shader);
		
		return shader;
	}

	Texture* OpenGLRendererAPI::CreateTexture(const LoadableTextureConfiguration& configuration) const
	{
		auto texture = new OpenGL::Texture(configuration);
		OpenGL::ObjectMap::Register(texture);

		return texture;
	}

	UniformBuffer* OpenGLRendererAPI::CreateUniformBuffer(const int64_t& size) const
	{
		return new OpenGLUniformBuffer(size);
	}

	VertexArray* OpenGLRendererAPI::CreateVertexArray(const VertexArrayData& data) const
	{
		return new OpenGLVertexArray(data);
	}

	Texture* OpenGLRendererAPI::GetTextureOf(Camera* camera) const
	{
		auto glCamera = OpenGL::ObjectMap::GetCamera(camera);
		return OpenGL::Utils::GLTextureToVarletTexture(glCamera->framebuffer.attachments[0]);
	}

	void OpenGLRendererAPI::ChangeResolution(Camera* camera) const
	{
		auto glCamera = OpenGL::ObjectMap::GetCamera(camera);

		int32_t width, height;
		camera->GetResolution(width, height);
		glCamera->Resize(width, height, camera->dynamicResolution);
	}

	void OpenGLRendererAPI::GetFramebufferSize(Camera* camera, int32_t& width, int32_t& height) const
	{
		auto glCamera = OpenGL::ObjectMap::GetCamera(camera);
		width = glCamera->framebuffer.width;
		height = glCamera->framebuffer.height;
	}

	std::vector<uint8_t> OpenGLRendererAPI::ReadRenderTexturePixels(Camera* camera, const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment) const
	{
		auto glCamera = OpenGL::ObjectMap::GetCamera(camera);
		return glCamera->ReadPixels(x, y, width, height, attachment);
	}

	// TODO rename NativeTextureOf
	const void* OpenGLRendererAPI::GetNativeRenderTexture(Camera* camera, const uint32_t& attachment) const
	{
		auto glCamera = OpenGL::ObjectMap::GetCamera(camera);
		return static_cast<const void*>(&glCamera->framebuffer.attachments[attachment]);
	}

	const void* OpenGLRendererAPI::GetNativeTexture(Texture* texture) const
	{
		return reinterpret_cast<const void*>(OpenGL::ObjectMap::GetTexture(texture));
	}
}
