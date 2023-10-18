#pragma once

#ifdef EDITOR_CORE_DLL_EXPORT
	#define EDITORCORE_API __declspec(dllexport)
#else
	#define EDITORCORE_API __declspec(dllimport)
#endif

#include "VarletCoreIncludes.h"
#include "EditorCoreCommonInclude.generated.h"
