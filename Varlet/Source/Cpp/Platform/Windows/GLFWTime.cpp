#include "GLFWTime.h"
#include "GLFW/glfw3.h"

namespace Varlet
{
	void GLFWTime::Update()
	{
		_now = static_cast<float>(glfwGetTime());
		_delta = _now - _lastUpdate;
		_lastUpdate = _now;
	}

	float GLFWTime::GetTime()
	{
		return _now;
	}

	float GLFWTime::GetDeltaTime()
	{
		return _delta;
	}
}
