#include "InputInternal.h"
#include "CoreInput.h"

namespace Varlet
{
	float InputInternal::_mouseX;
	float InputInternal::_mouseY;

	std::vector<KeyCode> InputInternal::_pressedKeys;
	std::vector<KeyCode> InputInternal::_releasedKeys;

	std::vector<MouseCode> InputInternal::_pressedMouse;
	std::vector<MouseCode> InputInternal::_releasedMouse;

	void InputInternal::Initialize()
	{
		static bool isInitialized = false;
		assert(isInitialized == false);

		Core::InputInitializer initializer{};
		initializer.keyCallback = InputInternal::KeyCallback;
		initializer.mouseCallback = InputInternal::MouseKeyCallback;
		initializer.mousePositionCallback = InputInternal::MousePositionCallback;

		Core::Input::Initialize(initializer);

		isInitialized = true;
	}

	void InputInternal::KeyCallback(const KeyCode& code, const KeyState& state)
	{
		const bool isPressed = state == KeyState::Pressed;
		auto& inputs = isPressed
			? _pressedKeys
			: _releasedKeys;

		AddKey(code, inputs);

		isPressed == false 
			? RemoveKey(code, _pressedKeys)
			: RemoveKey(code, _releasedKeys);
	}

	void InputInternal::MouseKeyCallback(const MouseCode& code, const KeyState& state)
	{
		const bool isPressed = state == KeyState::Pressed;
		auto& inputs = isPressed
			? _pressedMouse
			: _releasedMouse;

		AddKey(code, inputs);

		isPressed == false
			? RemoveKey(code, _pressedMouse)
			: RemoveKey(code, _releasedMouse);
	}

	void InputInternal::MousePositionCallback(const float& x, const float& y)
	{
		_mouseX = x;
		_mouseY = y;
	}
}
