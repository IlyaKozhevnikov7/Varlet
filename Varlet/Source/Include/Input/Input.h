#pragma once

#include "VarletCore.h"

enum class MouseCode
{
	Left = 0,
	Middle,
	Right,
	None
};

enum class KeyCode
{
	Space = 0,
	Apostrophe,	/* ' */
	Comma,		/* , */
	Minus,		/* - */
	Period,		/* . */
	Slash,		/* / */
	D0,
	D1,
	D2,
	D3,
	D4,
	D5,
	D6,
	D7,
	D8,
	D9,
	Semicolon,	/* ; */
	Equal,		/* = */
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	LeftBracket,	/* [ */
	Backslash,		/* \ */
	RightBracket,   /* ] */
	GraveAccent,	/* ` */
	World1,			/* non-US #1 */
	World2,			/* non-US #2 */
	Escape,
	Enter,
	Tab,
	Backspase,
	Insert,
	Delete,
	Right,
	Left,
	Down,
	Up,
	PageUp,
	PageDown,
	Home,
	End,
	CapsLock,
	ScrollLock,
	NumLock,
	PrintScreen,
	Pause,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,
	F25,
	KP0,
	KP1,
	KP2,
	KP3,
	KP4,
	KP5,
	KP6,
	KP7,
	KP8,
	KP9,
	KPDecimal,
	KPDivide,
	KPMultiply,
	KPSubtract,
	KPAdd,
	KPEnter,
	KPEqual,
	LeftShift,
	LeftControl,
	LeftAlt,
	LeftSuper,
	RightShift,
	RightControl,
	RightAlt,
	RightSuper,
	Menu,
	None
};

enum class KeyState
{
	Pressed = 0,
	Released
};

class CORE_API Input final
{
public:

	static bool GetKey(const KeyCode& keyCode, const KeyState& state);

	static bool GetMouse(const MouseCode& keyCode, const KeyState& state);

	static float GetMouseX();

	static float GetMouseY();

	static glm::vec2 GetMousePosition();
};

class InputInternal final
{
	friend Input;

	template<typename CodeType>
	struct InputType final
	{
		CodeType code;
		KeyState state;
	};

	using KeyInput = InputType<KeyCode>;
	using MouseInput = InputType<MouseCode>;

private:

	static float _mouseX;
	static float _mouseY;

	static std::vector<KeyCode> _pressedKeys;
	static std::vector<KeyCode> _releasedKeys;

	static std::vector<MouseCode> _pressedMouse;
	static std::vector<MouseCode> _releasedMouse;

public:

	static void Update();

	static void UpdateMousePosition(const float& x, const float& y);

	static void AddKey(const KeyCode& code, const KeyState& state);

	static void AddMouseButton(const MouseCode& code, const KeyState& state);

private:

	static void ResolveKeys();

	template<typename CodeType>
	static void AddEvent(const CodeType& code, std::vector<CodeType>& inputs)
	{
		bool wasReplaced = false;

		for (auto& input : inputs)
			if (input == CodeType::None)
			{
				input = code;
				wasReplaced = true;
				break;
			}

		if (wasReplaced == false)
			inputs.push_back(code);
	}
};

