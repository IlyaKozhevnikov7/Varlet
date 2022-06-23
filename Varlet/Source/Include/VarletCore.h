#pragma once

#ifdef DLL_EXPORT
	#define CORE_API __declspec(dllexport)
#else
	#define CORE_API __declspec(dllimport)
#endif // DLL_EXPORT

#include <stdint.h>
<<<<<<< HEAD

=======
#include <cassert>
#include <string>

#include "../../Dependencies/GLM/include/glm/glm.hpp"
>>>>>>> dev
#include "Logger.h"