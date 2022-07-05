#include "RendererAPI.h"

namespace Varlet
{
	IRendererAPI* RendererAPI::_api;

	void RendererAPI::Init(IRendererAPI* api)
	{
#ifdef DEBUG
		static bool initialized = false;
		assert(initialized == false && "RendererAPI already was initialized");
		assert(api != nullptr && "RendererAPI init argument was nullptr");
		initialized = true;
#endif // DEBUG

		_api = api;
	}

	Shader* RendererAPI::CreateShader(const ShaderInitializer& initializer)
	{
		return _api->CreateShader(initializer);
	}

	CameraCore* RendererAPI::CreateCameraCore()
	{
		return _api->CreateCameraCore();
	}

	Framebuffer* RendererAPI::CreateFrameBuffer()
	{
		return _api->CreateFrameBuffer();
	}

	Texture* RendererAPI::CreateTexture(const TextureConfiguration& configuration)
	{
		return _api->CreateTexture(configuration);
	}

	Texture* RendererAPI::LoadTexture(const LoadableTextureConfiguration& configuration)
	{
		return _api->LoadTexture(configuration);
	}

	UniformBuffer* RendererAPI::CreateUniformBuffer(const int64_t& size)
	{
		return _api->CreateUniformBuffer(size);
	}

	VertexArray* RendererAPI::CreateVertexArray(const VertexArrayData& configuration)
	{
		return _api->CreateVertexArray(configuration);
	}
}
