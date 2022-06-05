#pragma once

#ifdef DLL_EXPORT
	#define VARLET_API __declspec(dllexport)
#elif	
	#define VARLET_API __declspec(dllimport)
#endif // DLL_EXPORT

#include "Core/Module.h"

#define MAKE_TARGET_SYSTEM(SystemType) MAKE_TARGET_SYSTEM_IMPLEMENT(SystemType)
#define MAKE_TARGET_SYSTEM_IMPLEMENT(SystemType) Varlet::Module* targetModule = new SystemType();