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
}
