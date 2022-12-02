#include "RenderingCore.h"

#include "Entity.h"
#include "Component.h"
#include "Renderer.h"
#include "Transform.h"
#include "Camera.h"
#include "PointLight.h"

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
