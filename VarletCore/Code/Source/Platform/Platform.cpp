#include "Platform/WindowsPlatform.h"
#include "glfw/glfw3.h"
#include "Engine.h"
#include "CoreInput.h"
#include "Screen.h"
#include "CoreTime.h"

static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Varlet::Core::Input::UpdateMousePosition(xpos, ypos);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT)
		return;
	
	const KeyState state = action == GLFW_PRESS 
		? KeyState::Pressed 
		: KeyState::Released;

#define KEY_CASE(NativeKey, Key) case NativeKey : Varlet::Core::Input::AddKey(Key, state); break;

	switch (key)
	{
		KEY_CASE(GLFW_KEY_SPACE, KeyCode::Space);
		KEY_CASE(GLFW_KEY_APOSTROPHE, KeyCode::Apostrophe);
		KEY_CASE(GLFW_KEY_COMMA, KeyCode::Comma);
		KEY_CASE(GLFW_KEY_MINUS, KeyCode::Minus);
		KEY_CASE(GLFW_KEY_PERIOD, KeyCode::Period);
		KEY_CASE(GLFW_KEY_SLASH, KeyCode::Slash);
	
		KEY_CASE(GLFW_KEY_0, KeyCode::D0);
		KEY_CASE(GLFW_KEY_1, KeyCode::D1);
		KEY_CASE(GLFW_KEY_2, KeyCode::D2);
		KEY_CASE(GLFW_KEY_3, KeyCode::D3);
		KEY_CASE(GLFW_KEY_4, KeyCode::D4);
		KEY_CASE(GLFW_KEY_5, KeyCode::D5);
		KEY_CASE(GLFW_KEY_6, KeyCode::D6);
		KEY_CASE(GLFW_KEY_7, KeyCode::D7);
		KEY_CASE(GLFW_KEY_8, KeyCode::D8);
		KEY_CASE(GLFW_KEY_9, KeyCode::D9);
	
		KEY_CASE(GLFW_KEY_SEMICOLON, KeyCode::Semicolon);
		KEY_CASE(GLFW_KEY_EQUAL, KeyCode::Equal);
	
		KEY_CASE(GLFW_KEY_A, KeyCode::A);
		KEY_CASE(GLFW_KEY_B, KeyCode::B);
		KEY_CASE(GLFW_KEY_C, KeyCode::C);
		KEY_CASE(GLFW_KEY_D, KeyCode::D);
		KEY_CASE(GLFW_KEY_E, KeyCode::E);
		KEY_CASE(GLFW_KEY_F, KeyCode::F);
		KEY_CASE(GLFW_KEY_G, KeyCode::G);
		KEY_CASE(GLFW_KEY_H, KeyCode::H);
		KEY_CASE(GLFW_KEY_I, KeyCode::I);
		KEY_CASE(GLFW_KEY_J, KeyCode::J);
		KEY_CASE(GLFW_KEY_K, KeyCode::K);
		KEY_CASE(GLFW_KEY_L, KeyCode::L);
		KEY_CASE(GLFW_KEY_M, KeyCode::M);
		KEY_CASE(GLFW_KEY_N, KeyCode::N);
		KEY_CASE(GLFW_KEY_O, KeyCode::O);
		KEY_CASE(GLFW_KEY_P, KeyCode::P);
		KEY_CASE(GLFW_KEY_Q, KeyCode::Q);
		KEY_CASE(GLFW_KEY_R, KeyCode::R);
		KEY_CASE(GLFW_KEY_S, KeyCode::S);
		KEY_CASE(GLFW_KEY_T, KeyCode::T);
		KEY_CASE(GLFW_KEY_U, KeyCode::U);
		KEY_CASE(GLFW_KEY_V, KeyCode::V);
		KEY_CASE(GLFW_KEY_W, KeyCode::W);
		KEY_CASE(GLFW_KEY_X, KeyCode::X);
		KEY_CASE(GLFW_KEY_Y, KeyCode::Y);
		KEY_CASE(GLFW_KEY_Z, KeyCode::Z);
	
		// TODO precess all inputs
	}

#undef KEY_CASE(NativeKey, Key)
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	const KeyState state = action == GLFW_PRESS 
		? KeyState::Pressed 
		: KeyState::Released;
	
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:	Varlet::Core::Input::AddMouse(MouseCode::Left, state);	break;
	case GLFW_MOUSE_BUTTON_RIGHT:	Varlet::Core::Input::AddMouse(MouseCode::Right, state);	break;
	case GLFW_MOUSE_BUTTON_MIDDLE:	Varlet::Core::Input::AddMouse(MouseCode::Middle, state);	break;
	}
}

static void WindowCloseCallback(GLFWwindow* window)
{
	Varlet::Core::Engine::WantExit();
}

namespace Varlet::Core
{
	HMODULE Platform::_module;

	void Platform::Init()
	{
		_module = GetModuleHandle(nullptr);
	}

	void Platform::Update()
	{
		glfwPollEvents();
		TimeInternal::UpdateTime(glfwGetTime());
	}

	void Platform::CreateWindow()
	{
		static bool wasCreated = false;
		assert(wasCreated == false);

		if (glfwInit() == 1)
		{
			wasCreated = true;
		}
		else
		{
			//VARLET_ERROR("Varlet support only one window");
		}

		const auto window = glfwCreateWindow(1280, 720, "Varlet Window", nullptr, nullptr);
		
		//if (window == nullptr)
		//	VARLET_ERROR("Failed to create window");

		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);

		if (Input::IsSupportKeyboard())
			glfwSetKeyCallback(window, KeyCallback);

		if (Input::IsSupportMouse())
		{
			glfwSetCursorPosCallback(window, CursorPosCallback);
			glfwSetMouseButtonCallback(window, MouseButtonCallback);
		}

		glfwSetWindowCloseCallback(window, WindowCloseCallback);

		ScreenInternal::Initialize(window);
	}

	Platform::Module Platform::LoadModule(const char* name)
	{
		auto module = LoadLibraryA(name);
		
		if (module == nullptr)
			VARLET_LOG(Error, "Loading module " + *name + *" was failed...");

		return module;
	}

	void* Platform::LoadFunction(Platform::Module& module, const char* name)
	{
		void* function = GetProcAddress(module, name);
		if (function == nullptr)
			VARLET_LOG(LevelType::Error, "Failed to load " + *name + *"() function");

		return function;
	}

	bool Platform::FreeModule(Platform::Module& module)
	{
		return FreeLibrary(module);
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