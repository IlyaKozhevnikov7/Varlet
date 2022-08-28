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

	Framebuffer* RendererAPI::CreateFrameBuffer(FramebufferConfiguration& configuration)
	{
		return _api->CreateFrameBuffer(configuration);
	}

	Texture* RendererAPI::CreateTexture(const TextureConfiguration& configuration)
	{
		return _api->CreateTexture(configuration);
	}

	std::shared_ptr<Texture> RendererAPI::LoadTexture(const LoadableTextureConfiguration& configuration)
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
	
	void RendererAPI::UnbindTexure(const int32_t& unit)
	{
		_api->UnbindTexure(unit);
	}
}
