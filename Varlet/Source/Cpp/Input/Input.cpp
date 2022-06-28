#include "Input.h"

Varlet::InputModule* Input::_core;

bool Input::Init(Varlet::InputModule* core)
{
	_core = core;

	return _core != nullptr ? SUCCESSFUL_INITIALIZATION : FAILED_INITIALIZATION;
}

bool Input::GetKey(const int32_t& keyCode, const int32_t& state)
{
	return _core->GetKey(keyCode, state);
}

bool Input::GetMouse(const int32_t& keyCode, const int32_t& state)
{
	return false;
}

float Input::GetMouseX()
{
	return 0.0f;
}

float Input::GetMouseY()
{
	return 0.0f;
}

glm::vec2 Input::GetMousePosition()
{
	return glm::vec2();
}
