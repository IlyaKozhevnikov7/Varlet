#include "GLFWContext.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Varlet
{
	int32_t GLFWContext::Init()
	{
		int32_t initResult = glfwInit();

		if (initResult)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			_window = glfwCreateWindow(1280, 720, "Varlet", NULL, NULL);

			if (_window == nullptr)
			{
				glfwTerminate();
				return GLFW_FALSE;
			}

			glfwMakeContextCurrent(_window);
			glfwSwapInterval(0);
		}

		return initResult;
	}

	bool GLFWContext::IsRunning() const
	{
		return glfwWindowShouldClose(_window) == false;
	}

	void GLFWContext::Update()
	{
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	void GLFWContext::Shutdown()
	{
		glfwDestroyWindow(_window);
		_window = nullptr;
		glfwTerminate();
	}

	void GLFWContext::SetCursorState(const CursorState& state) const
	{
		int32_t glfwState;

		switch (state)
		{	
		case CursorState::Visible:
			glfwState = GLFW_CURSOR_NORMAL;
			break;

		case CursorState::Disabled:
			glfwState = GLFW_CURSOR_DISABLED;
			break;

		case CursorState::Hidden:
			glfwState = GLFW_CURSOR_HIDDEN;
			break;
		}

		glfwSetInputMode(_window, GLFW_CURSOR, glfwState);
	}
}
