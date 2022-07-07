#include "GLFWTime.h"
#include "GLFW/glfw3.h"

namespace Varlet
{
	void GLFWTime::UpdateTime()
	{
		const float now = static_cast<float>(glfwGetTime());
		_delta = now - _lastUpdate;
		_lastUpdate = now;
	}

	float GLFWTime::GetTime()
	{
		return glfwGetTime();
	}

	float GLFWTime::GetDeltaTime()
	{
		return _delta;
	}
}
