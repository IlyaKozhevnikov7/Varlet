#include "GLFWContext.h"

#include "../../Dependencies/GLFW/glfw3.h"
#include "../../Dependencies/GLFW/glfw3native.h"

namespace Varlet
{
	int32_t GLFWContext::Init()
	{
		int32_t initResult = glfwInit();
		
		if (initResult != 0)
		{
			_window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
			glfwMakeContextCurrent(_window);
		}
		
		return 1;
	}
	
	void GLFWContext::Shutdown()
	{
		glfwDestroyWindow(_window);
		_window = nullptr;
		glfwTerminate();
	}
}
