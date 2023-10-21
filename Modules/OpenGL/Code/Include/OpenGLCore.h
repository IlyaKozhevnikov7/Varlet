#pragma once

#ifdef OPENGL_DLL_EXPORT
	#define OPENGL_API __declspec(dllexport)
#else
	#define OPENGL_API __declspec(dllimport)
#endif

#include "VarletCoreIncludes.h"
#include "OpenGLCommonInclude.generated.h"
