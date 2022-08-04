#pragma once

#ifdef VARLET_DLL_EXPORT
	#define CORE_API __declspec(dllexport)
#else
	#define CORE_API __declspec(dllimport)
#endif // DLL_EXPORT

#include <stdint.h>
#include <cassert>
#include <string>

#include "../../Dependencies/GLM/include/GLM/glm.hpp"
#include "../../Dependencies/GLM/include/GLM/gtc/type_ptr.hpp"
#include "../../Dependencies/GLM/include/GLM/gtx/matrix_decompose.hpp"
#include "Core/Delegate.h"
#include "Core/Logger.h"