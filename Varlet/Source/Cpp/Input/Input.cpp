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
	return _core->GetMouse(keyCode, state);
}

float Input::GetMouseX()
{
	return _core->GetMouseX();
}

float Input::GetMouseY()
{
	return _core->GetMouseY();
}

glm::vec2 Input::GetMousePosition()
{
	return _core->GetMousePosition();
}
