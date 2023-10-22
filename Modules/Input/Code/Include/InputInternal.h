#pragma once

#include <vector>

enum class KeyCode;
enum class MouseCode;
enum class KeyState;

namespace Varlet
{
	class InputInternal final
	{
		friend class Input;

	private:

		static float _mouseX;
		static float _mouseY;

		static std::vector<KeyCode> _pressedKeys;
		static std::vector<KeyCode> _releasedKeys;

		static std::vector<MouseCode> _pressedMouse;
		static std::vector<MouseCode> _releasedMouse;

	public:

		static void Initialize();

	private:

		static void KeyCallback(const KeyCode& code, const KeyState& state);

		static void MouseKeyCallback(const MouseCode& code, const KeyState& state);

		static void MousePositionCallback(const float& x, const float& y);

		template<typename CodeType>
		static void AddKey(const CodeType& code, std::vector<CodeType>& inputs)
		{
			for (auto& input : inputs)
				if (input == CodeType::None)
				{
					input = code;
					return;
				}

			inputs.push_back(code);
		}

		template<typename CodeType>
		static void RemoveKey(const CodeType& code, std::vector<CodeType>& inputs)
		{
			for (auto& input : inputs)
				if (input == code)
				{
					input = CodeType::None;
					return;
				}
		}

		template<typename CodeType>
		static void GetKey(const CodeType& code, std::vector<CodeType>& inputs)
		{
			for (auto& input : inputs)
				if (input == code)
				{
					input = code;
					return;
				}
		}
	};
}
