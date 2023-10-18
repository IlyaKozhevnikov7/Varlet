#pragma once

#ifdef VARLET_DLL_EXPORT
	#define VARLET_API __declspec(dllexport)
#else
	#define VARLET_API __declspec(dllimport)
#endif // VARLET_DLL_EXPORT

#include <array>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <stdint.h>
#include <cassert>
#include <string>

#include "../../Dependencies/GLM/include/GLM/glm.hpp"
#include "../../Dependencies/GLM/include/GLM/vector_relational.hpp"
#include "../../Dependencies/GLM/include/GLM/gtc/type_ptr.hpp"
#include "../../Dependencies/GLM/include/GLM/gtx/matrix_decompose.hpp"

#include "VarletCoreIncludes.h"
#include "VarletCommonInclude.generated.h"

__GEN_REGISTER_TYPE_NAME_INTERNAL(glm::vec2);
__GEN_REGISTER_TYPE_NAME_INTERNAL(glm::vec3);
__GEN_REGISTER_TYPE_NAME_INTERNAL(glm::vec4);
__GEN_REGISTER_TYPE_NAME_INTERNAL(glm::mat2);
__GEN_REGISTER_TYPE_NAME_INTERNAL(glm::mat3);
__GEN_REGISTER_TYPE_NAME_INTERNAL(glm::mat4);
__GEN_REGISTER_TYPE_NAME_INTERNAL(glm::quat);
