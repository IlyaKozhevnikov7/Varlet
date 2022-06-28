#pragma once

#include "VarletCore.h"
#include "InputModule.h"

namespace Varlet
{
	class InputModule;
}

class Input final
{
private:

	static Varlet::InputModule* _core;

public:

	static bool Init(Varlet::InputModule* core);

	CORE_API static bool GetKey(const int32_t& keyCode, const int32_t& state);

	CORE_API static bool GetMouse(const int32_t& keyCode, const int32_t& state);

	CORE_API static float GetMouseX();

	CORE_API static float GetMouseY();

	CORE_API static glm::vec2 GetMousePosition();
};

