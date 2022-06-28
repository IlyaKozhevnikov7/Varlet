#include "GLFWInput.h"
#include "VarletAPI.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Varlet
{
	int32_t GLFWInput::Init()
	{
		int32_t result = InputModule::Init();

		_window = VarletAPI::GetContextWindow<GLFWwindow>();
		if (_window == nullptr)
			return FAILED_INITIALIZATION;

		return result;
	}

	bool GLFWInput::GetKey(const int32_t& keyCode, const int32_t& state) const
	{
		return glfwGetKey(_window, keyCode) == state;
	}

	bool GLFWInput::GetMouse(const int32_t& keyCode, const int32_t& state) const
	{
		return false;
	}

	float GLFWInput::GetMouseX() const
	{
		return 0.0f;
	}

	float GLFWInput::GetMouseY() const
	{
		return 0.0f;
	}

	glm::vec2 GLFWInput::GetMousePosition() const
	{
		return glm::vec2();
	}
}
