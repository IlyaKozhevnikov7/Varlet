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
		return glfwGetMouseButton(_window, keyCode) == state;
	}

	float GLFWInput::GetMouseX() const
	{
		return GetMousePosition().x;
	}

	float GLFWInput::GetMouseY() const
	{
		return GetMousePosition().y;
	}

	glm::vec2 GLFWInput::GetMousePosition() const
	{
		double x, y;
		glfwGetCursorPos(_window, &x, &y);

		return glm::vec2(x, y);
	}
}
