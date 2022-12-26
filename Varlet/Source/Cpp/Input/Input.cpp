#include "Input.h"

bool Input::GetKey(const KeyCode& keyCode, const KeyState& state)
{
	if (keyCode == KeyCode::None)
		return false;

	const auto& inputs = state == KeyState::Pressed ? InputInternal::_pressedKeys : InputInternal::_releasedKeys;

	for (const auto& input : inputs)
		if (input == keyCode)
			return true;

	return false;
}

bool Input::GetMouse(const MouseCode& keyCode, const KeyState& state)
{
	if (keyCode == MouseCode::None)
		return false;

	const auto& inputs = state == KeyState::Pressed ? InputInternal::_pressedMouse : InputInternal::_releasedMouse;

	for (const auto& input : inputs)
		if (input == keyCode)
			return true;

	return false;
}

float Input::GetMouseX()
{
	return InputInternal::_mouseX;
}

float Input::GetMouseY()
{
	return InputInternal::_mouseY;
}

glm::vec2 Input::GetMousePosition()
{
	return { InputInternal::_mouseX, InputInternal::_mouseY };
}

float InputInternal::_mouseX;
float InputInternal::_mouseY;

std::vector<KeyCode> InputInternal::_pressedKeys;
std::vector<KeyCode> InputInternal::_releasedKeys;

std::vector<MouseCode> InputInternal::_pressedMouse;
std::vector<MouseCode> InputInternal::_releasedMouse;

void InputInternal::Update()
{
	ResolveKeys();
}

void InputInternal::UpdateMousePosition(const float& x, const float& y)
{
	_mouseX = x;
	_mouseY = y;
}

void InputInternal::AddKey(const KeyCode& code, const KeyState& state)
{
	auto& inputs = state == KeyState::Pressed ? _pressedKeys : _releasedKeys;

	AddEvent(code, inputs);
}

void InputInternal::AddMouseButton(const MouseCode& code, const KeyState& state)
{
	auto& inputs = state == KeyState::Pressed ? _pressedMouse : _releasedMouse;

	AddEvent(code, inputs);
}

void InputInternal::ResolveKeys()
{
	for (auto& releasedCode : _releasedKeys)
	{
		if (releasedCode == KeyCode::None)
			continue;

		for (auto& pressedCode : _pressedKeys)
		{
			if (pressedCode == releasedCode)
			{
				pressedCode = KeyCode::None;
				releasedCode = KeyCode::None;
				break;
			}
		}
	}
}
