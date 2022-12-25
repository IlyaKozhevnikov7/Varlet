#include "Rendering/Graphics.h"
#include "Rendering/RendererAPI.h"

const uint8_t* GraphicsInfo::rendererName;
double GraphicsInfo::renderTime;

namespace Varlet
{
	int32_t Graphics::Init()
	{
		if (auto rendererAPIInitializer = dynamic_cast<IRendererAPIInitializerBase*>(this))
		{
			rendererAPIInitializer->InitRendererAPI();
			return SUCCESSFUL_INITIALIZATION;
		}
		
		return FAILED_INITIALIZATION;
	}
}
