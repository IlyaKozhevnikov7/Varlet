#include "CoreInput.h"

namespace Varlet::Core
{
	Input::KeyCallback Input::_keyCallback;
	Input::MouseCallback Input::_mouseCallback;
	Input::MousePositionCallback Input::_mousePositionCallback;
	float Input::_mouseX;
	float Input::_mouseY;

	void Input::Initialize(const InputInitializer& initializer)
	{
		static bool isInitialized = false;
		assert(isInitialized == false);

		if (isInitialized)
		{
			VARLET_LOG(Warning, "Input already initialized...");
			return;
		}

		_keyCallback = initializer.keyCallback;
		_mouseCallback = initializer.mouseCallback;
		_mousePositionCallback = initializer.mousePositionCallback;
		isInitialized = true;
	}

	bool Input::IsSupportKeyboard()
	{
		return _keyCallback != nullptr;
	}

	bool Input::IsSupportMouse()
	{
		return _mouseCallback != nullptr && _mousePositionCallback != nullptr;
	}

	void Input::AddKey(const KeyCode& code, const KeyState& state)
	{
		assert(_keyCallback);
		_keyCallback(code, state);
	}

	void Input::AddMouse(const MouseCode& code, const KeyState& state)
	{
		assert(_mouseCallback);
		_mouseCallback(code, state);
	}

	void Input::UpdateMousePosition(const float& x, const float& y)
	{
		assert(_mousePositionCallback);
		_mousePositionCallback(x, y);
	}
}