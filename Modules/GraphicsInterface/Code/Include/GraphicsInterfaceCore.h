#pragma once

#ifdef GRAPHICSINTERFACE_DLL_EXPORT
	#define GRAPHICSINTERFACE_API __declspec(dllexport)
#else
	#define GRAPHICSINTERFACE_API __declspec(dllimport)
#endif // GRAPHICSINTERFACE_API

#include "VarletCoreIncludes.h"
#include "GraphicsInterfaceCommonInclude.generated.h"
