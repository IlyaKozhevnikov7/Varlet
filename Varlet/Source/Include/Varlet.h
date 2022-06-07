#pragma once

#ifdef DLL_EXPORT
	#define CORE_API __declspec(dllexport)
#elif	
	#define CORE_API __declspec(dllimport)
#endif // DLL_EXPORT

#include "Core/Module.h"
#include "Core/Logger.h"

#include <vector>
#include <string>
#include <unordered_map>

#define MAKE_TARGET_SYSTEM(SystemType) MAKE_TARGET_SYSTEM_IMPLEMENT(SystemType)
#define MAKE_TARGET_SYSTEM_IMPLEMENT(SystemType) Varlet::Module* targetModule = new SystemType();