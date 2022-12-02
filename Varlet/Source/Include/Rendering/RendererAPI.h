#pragma once

#include "VarletCore.h"

class Camera;

struct TextureConfiguration;
struct LoadableTextureConfiguration;
struct FramebufferConfiguration;
struct ShaderInitializer;

enum class WrapType : uint8_t;
enum class FilterType : uint8_t;
enum class TextureFormat : uint8_t;

namespace Varlet
{
	struct LoadableTextureConfiguration;

	class UniformBuffer;
	class Shader;
	class Framebuffer;
	class Texture;
	class VertexArray;

	struct VertexArrayData;

	class IRendererAPI
	{
	public:

		virtual ~IRendererAPI() = default;

		virtual Shader* CreateShader(const ShaderInitializer& initializer) const = 0;

		virtual UniformBuffer* CreateUniformBuffer(const int64_t& size) const = 0;

		virtual Texture* CreateTexture(const LoadableTextureConfiguration& configuration) const = 0;

		virtual VertexArray* CreateVertexArray(const VertexArrayData& configuration) const = 0;

		virtual Texture* GetTextureOf(Camera* camera) const = 0;

		virtual void ChangeResolution(Camera* camera) const = 0;

		virtual void GetFramebufferSize(Camera* camera, int32_t& width, int32_t& height) const = 0;

		virtual std::vector<uint8_t> ReadRenderTexturePixels(Camera* camera, const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment = 0) const = 0;

		virtual const void* GetNativeRenderTexture(Camera* camera, const uint32_t& attachment = 0) const = 0;

		virtual const void* GetNativeTexture(Texture* texture) const = 0;
	};

	class RendererAPI final
	{
	private:

		static IRendererAPI* _api;

	public:

		static void Init(IRendererAPI* api);

		CORE_API static Shader* CreateShader(const ShaderInitializer& initializer);

		CORE_API static Texture* CreateTexture(const LoadableTextureConfiguration& configuration);

		CORE_API static UniformBuffer* CreateUniformBuffer(const int64_t& size);

		CORE_API static VertexArray* CreateVertexArray(const VertexArrayData& configuration);

		// internal API 

		CORE_API static Texture* GetTextureOf(Camera* camera);

		CORE_API static void ChangeResolution(Camera* camera);

		CORE_API static void GetFramebufferSize(Camera* camera, int32_t& width, int32_t& height);

		CORE_API static std::vector<uint8_t> ReadRenderTexturePixels(Camera* camera, const int32_t& x, const int32_t& y, const int32_t& width = 1, const int32_t& height = 1, const uint32_t& attachment = 0);

		CORE_API static const void* GetNativeRenderTexture(Camera* camera, const uint32_t& attachment = 0);

		CORE_API static const void* GetNativeTexture(Texture* texture);
	};

	class IRendererAPIInitializerBase
	{
	public:

		virtual void InitRendererAPI() = 0;
		
		virtual ~IRendererAPIInitializerBase() = default;
	};

	template<class T>
	class IRendererAPIInitializer : public IRendererAPIInitializerBase
	{
	public:

		void InitRendererAPI() override
		{
			static_assert(std::is_base_of<IRendererAPI, T>::value, "IRendererAPIInitializer must have Renderer template type");

			RendererAPI::Init(new T());
		}

		~IRendererAPIInitializer() override = default;
	};
}
