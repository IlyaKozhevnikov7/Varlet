#include "GLFWContext.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Varlet
{
	int32_t GLFWContext::Init()
	{
		int32_t initResult = glfwInit();
		
		if (initResult != 0)
		{
			_window = glfwCreateWindow(640, 480, "Varlet", nullptr, nullptr);
			glfwMakeContextCurrent(_window);
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
}
