#pragma once

#include "VarletCore.h"
#include "Renderer.h"

namespace Varlet
{
	class IRendererAPI
	{
	public:

		// create shader
		// create texture
		// create framebuffer
		// create ...
	};

	class RendererAPI
	{
	private:

		static IRendererAPI* _api;

	public:

		RendererAPI() = default;
		virtual ~RendererAPI() = default;

		static void Init(IRendererAPI* api)
		{
#ifdef DEBUG
			static bool initialized = false;
			assert(initialized == false && "RendererAPI already was initialized");
			assert(api != nullptr && "RendererAPI init argument was nullptr");
			initialized = true;
#endif // DEBUG

			_api = api;
		}
	};

	class IRendererAPIInitializerBase
	{
	public:

		virtual void InitRendererAPI() = 0;

		virtual ~IRendererAPIInitializerBase() = default;
	};

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
