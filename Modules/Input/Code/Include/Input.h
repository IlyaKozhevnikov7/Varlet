#pragma once

#include "InputCore.h"
#include "CoreInput.h"

class INPUT_API Input final
{
public:

	static bool GetKey(const KeyCode& keyCode, const KeyState& state = KeyState::Pressed);

	static bool GetMouse(const MouseCode& keyCode, const KeyState& state = KeyState::Pressed);

	static float GetMouseX();

	static float GetMouseY();
};
