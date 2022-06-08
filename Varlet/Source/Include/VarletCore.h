#pragma once

#ifdef DLL_EXPORT
	#define CORE_API __declspec(dllexport)
#elif	
	#define CORE_API __declspec(dllimport)
#endif // DLL_EXPORT

#include <stdint.h>