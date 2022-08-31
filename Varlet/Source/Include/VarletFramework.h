#pragma once

// math
#include "../../Dependencies/GLM/include/glm/glm.hpp"
#include "../../Dependencies/GLM/include/GLM/ext/matrix_transform.hpp"

// utils
#include "Core/Delegate.h"
#include "Core/Logger.h"
#include "Core/Reflection.h"
#include "Core/Object.h"

// components
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/Component.h"
#include "Scene/Script.h"
#include "Scene/Camera.h"
#include "Scene/MeshRenderer.h"
#include "Scene/Transform.h"
#include "Scene/PointLight.h"

// input
#include "Input/Input.h"
#include "Platform/Windows/GLFWInput.h" // for key enums

#include "Time/VarletTime.h"