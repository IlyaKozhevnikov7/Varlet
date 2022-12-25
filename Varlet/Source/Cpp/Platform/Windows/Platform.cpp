#include "Windows/Platform.h"
#include "GLFW/glfw3.h"
#include "Engine.h"
#include "Core/Screen.h"
#include "Input/Input.h"

static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
}

static void WindowCloseCallback(GLFWwindow* window)
{
	Varlet::Engine::WantExit();
}

namespace Varlet
{
	HMODULE Platform::_module;

	void Platform::Init()
	{
		_module = GetModuleHandle(nullptr);
	}

	void Platform::Update()
	{
		glfwPollEvents();
	}

	void Platform::CreateContext()
	{
		static bool wasCreated = false;
		assert(wasCreated == false);

		if (glfwInit() == GLFW_TRUE)
		{
			wasCreated = true;
		}
		else
		{
			VARLET_ERROR("Varlet support only one window");
		}

		const auto window = glfwCreateWindow(1280, 720, "Varlet Window", nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);

		glfwSetCursorPosCallback(window, CursorPosCallback);
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetWindowCloseCallback(window, WindowCloseCallback);

		ScreenInternal::Init(window);
	}

	void PlatformGL::MakeCurrent(const Platform::WindowDescriptor context)
	{
		glfwMakeContextCurrent(context);
	}

	void PlatformGL::SwapBuffers(const Platform::WindowDescriptor context)
	{
		glfwSwapBuffers(context);
	}
}