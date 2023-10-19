#pragma once

#ifdef GRAPHICSCORE_DLL_EXPORT
	#define GRAPHICSCORE_API __declspec(dllexport)
#else
	#define GRAPHICSCORE_API __declspec(dllimport)
#endif // GRAPHICSCORE_DLL_EXPORT

#include "VarletCoreIncludes.h"

#ifdef VARLET_DEVELOPMENT
	#include "EditorCore.h"
#endif // VARLET_DEVELOPMENT

#include "GraphicsCoreCommonInclude.generated.h"