#include "OpenGL/RendererAPI.h"

// new
#include "OpenGL/DescriptorPool.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Utils.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"
#include "RendererAPI.h"

#include "Scene/Camera.h"

namespace Varlet::OpenGL
{
	Varlet::Shader* RendererAPI::CreateShader(const std::string& vertSource, const std::string& fragSource, const std::string& geomSource) const
	{
		auto shader = new OpenGL::Shader(vertSource, fragSource, geomSource);
		OpenGL::DescriptorPool::Register(shader);
		
		return shader;
	}

	Varlet::Texture* RendererAPI::CreateTexture(const LoadableTextureConfiguration& configuration) const
	{
		auto texture = new OpenGL::Texture(configuration);
		OpenGL::DescriptorPool::Register(texture);

		return texture;
	}

	void RendererAPI::RegisterMesh(const Mesh* mesh, const std::vector<MeshData>& vertices) const
	{
		DescriptorPool::Commit(mesh, vertices);
	}

	Varlet::Texture* RendererAPI::GetTextureOf(::Camera* camera) const
	{
		auto glCamera = OpenGL::DescriptorPool::GetCamera(camera);
		return OpenGL::Utils::GLTextureToVarletTexture(glCamera->framebuffer.attachments[0]);
	}

	void RendererAPI::ChangeResolution(::Camera* camera) const
	{
		auto glCamera = OpenGL::DescriptorPool::GetCamera(camera);

		int32_t width, height;
		camera->GetResolution(width, height);
		glCamera->Resize(width, height, camera->dynamicResolution);
	}

	void RendererAPI::GetFramebufferSize(::Camera* camera, int32_t& width, int32_t& height) const
	{
		auto glCamera = OpenGL::DescriptorPool::GetCamera(camera);
		width = glCamera->framebuffer.width;
		height = glCamera->framebuffer.height;
	}

	std::vector<uint8_t> RendererAPI::ReadRenderTexturePixels(::Camera* camera, const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment) const
	{
		auto glCamera = OpenGL::DescriptorPool::GetCamera(camera);
		return glCamera->ReadPixels(x, y, width, height, attachment);
	}

	// TODO rename NativeTextureOf
	const void* RendererAPI::GetNativeRenderTexture(::Camera* camera, const uint32_t& attachment) const
	{
		auto glCamera = OpenGL::DescriptorPool::GetCamera(camera);
		return static_cast<const void*>(&glCamera->framebuffer.attachments[attachment]);
	}

	const void* RendererAPI::GetNativeTexture(Varlet::Texture* texture) const
	{
		return reinterpret_cast<const void*>(OpenGL::DescriptorPool::GetNativeTexture(texture));
	}

	void RendererAPI::DestroyCamera(::Camera* camera) const
	{
		DescriptorPool::Unregister(camera);
	}
}
