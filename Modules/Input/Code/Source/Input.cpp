#include "Input.h"
#include "CoreInput.h"
#include "InputInternal.h"

bool Input::GetKey(const KeyCode& keyCode, const KeyState& state)
{
	auto& inputs = state == KeyState::Pressed
		? Varlet::InputInternal::_pressedKeys
		: Varlet::InputInternal::_releasedKeys;

	for (auto& input : inputs)
		if (input == keyCode)
			return true;

	return false;
}

bool Input::GetMouse(const MouseCode& keyCode, const KeyState& state)
{
	auto& inputs = state == KeyState::Pressed
		? Varlet::InputInternal::_pressedMouse
		: Varlet::InputInternal::_releasedMouse;

	for (auto& input : inputs)
		if (input == keyCode)
			return true;

	return false;
}

float Input::GetMouseX()
{
	return Varlet::InputInternal::_mouseX;
}

float Input::GetMouseY()
{
	return Varlet::InputInternal::_mouseY;
}
