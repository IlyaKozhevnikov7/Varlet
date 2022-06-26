#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class IRendererAPI
	{
	public:

		//virtual CameraCore* CreateCamera() = 0;

		// create shader
		// create texture
		// create framebuffer
		// create ...
	};

	class RendererAPI final
	{
	private:

		static IRendererAPI* _api;

	public:

		static void Init(IRendererAPI* api);
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
