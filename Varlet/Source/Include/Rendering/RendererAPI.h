#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class CameraCore;
	class UniformBuffer;
	class Shader;
	class Framebuffer;

	struct ShaderInitializer;

	class IRendererAPI
	{
	public:

		virtual Shader* CreateShader(const ShaderInitializer* initializer) const = 0;

		virtual CameraCore* CreateCameraCore() const = 0;

		virtual Framebuffer* CreateFrameBuffer() const = 0;

		virtual UniformBuffer* CreateUniformBuffer(const int64_t& size) const = 0;

		// create texture
		// create ...
	};

	class RendererAPI final
	{
	private:

		static IRendererAPI* _api;

	public:

		static void Init(IRendererAPI* api);

		static Shader* CreateShader(const ShaderInitializer* initializer);

		static CameraCore* CreateCameraCore();

		static Framebuffer* CreateFrameBuffer();

		static UniformBuffer* CreateUniformBuffer(const int64_t& size);
	};

	class IRendererAPIInitializerBase
	{
	public:

		virtual void InitRendererAPI() = 0;

		virtual ~IRendererAPIInitializerBase() = default;
	};

	// TODO maybe remove exporting 
	template<class T>
	class CORE_API IRendererAPIInitializer : public IRendererAPIInitializerBase
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
