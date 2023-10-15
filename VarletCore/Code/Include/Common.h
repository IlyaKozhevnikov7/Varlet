#pragma once

#ifdef CORE_DLL_EXPORT
	#define CORE_API __declspec(dllexport)
#else
	#define CORE_API __declspec(dllimport)
#endif // CORE_DLL_EXPORT

#include <vector>
#include <unordered_map>
#include <string>
#include <array>
#include <stack>

#include <locale>
#include <codecvt>

#include "BaseUtils.h"
#include "Logger.h"
#include "Delegate.h"
#include "../../../Modules/Varlet/Dependencies/GLM/include/GLM/glm.hpp"

#include "VarletCoreCommonInclude.generated.h"