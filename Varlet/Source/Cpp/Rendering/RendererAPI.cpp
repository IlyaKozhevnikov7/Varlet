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

	Texture* RendererAPI::CreateTexture(const LoadableTextureConfiguration& configuration)
	{
		return _api->CreateTexture(configuration);
	}

	UniformBuffer* RendererAPI::CreateUniformBuffer(const int64_t& size)
	{
		return _api->CreateUniformBuffer(size);
	}

	VertexArray* RendererAPI::CreateVertexArray(const VertexArrayData& configuration)
	{
		return _api->CreateVertexArray(configuration);
	}

	Texture* RendererAPI::GetTextureOf(Camera* camera)
	{
		return _api->GetTextureOf(camera);
	}

	void RendererAPI::ChangeResolution(Camera* camera)
	{
		return _api->ChangeResolution(camera);
	}

	void RendererAPI::GetFramebufferSize(Camera* camera, int32_t& width, int32_t& height)
	{
		return _api->GetFramebufferSize(camera, width, height);
	}

	std::vector<uint8_t> RendererAPI::ReadRenderTexturePixels(Camera* camera, const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment)
	{
		return _api->ReadRenderTexturePixels(camera, x, y, width, height, attachment);
	}

	const void* RendererAPI::GetNativeTexture(Texture* texture)
	{
		return _api->GetNativeTexture(texture);
	}

	const void* RendererAPI::GetNativeRenderTexture(Camera* camera, const uint32_t& attachment)
	{
		return _api->GetNativeRenderTexture(camera, attachment);
	}
}
