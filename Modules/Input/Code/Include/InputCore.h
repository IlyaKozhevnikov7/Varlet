#pragma once

#ifdef INPUT_DLL_EXPORT
	#define INPUT_API __declspec(dllexport)
#else
	#define INPUT_API __declspec(dllimport)
#endif // INPUT_DLL_EXPORT