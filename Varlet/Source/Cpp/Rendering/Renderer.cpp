#include "Renderer.h"
<<<<<<< HEAD
=======
#include "RendererAPI.h"

namespace Varlet
{
	int32_t Renderer::Init()
	{
		if (auto rendererAPIInitializer = dynamic_cast<IRendererAPIInitializerBase*>(this))
		{
			rendererAPIInitializer->InitRendererAPI();
			return SUCCESSFUL_INITIALIZATION;
		}

		return FAILED_INITIALIZATION;
	}
}
>>>>>>> dev
