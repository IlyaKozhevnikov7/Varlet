#include "Platform/WindowsPlatform.h"
#include "Engine.h"
#include "CoreInput.h"
#include "Screen.h"
#include "CoreTime.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw/glfw3.h"
#include "glfw/glfw3native.h"

#define CHECK_PLATFORM_ERROR \
	const DWORD errorCore = GetLastError(); \
	if (errorCore > 0) \
	{ \
		LPWSTR errorMessage = nullptr; \
		FormatMessage(\
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
			NULL, \
			errorCore, \
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
			(LPWSTR)&errorMessage, \
			0, \
			NULL \
		); \
	} \

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
		KEY_CASE(GLFW_KEY_SPACE,		KeyCode::Space);
		KEY_CASE(GLFW_KEY_APOSTROPHE,	KeyCode::Apostrophe);
		KEY_CASE(GLFW_KEY_COMMA,		KeyCode::Comma);
		KEY_CASE(GLFW_KEY_MINUS,		KeyCode::Minus);
		KEY_CASE(GLFW_KEY_PERIOD,		KeyCode::Period);
		KEY_CASE(GLFW_KEY_SLASH,		KeyCode::Slash);
	
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
	
		KEY_CASE(GLFW_KEY_SEMICOLON,	KeyCode::Semicolon);
		KEY_CASE(GLFW_KEY_EQUAL,		KeyCode::Equal);
	
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
	
		KEY_CASE(GLFW_KEY_ESCAPE,		KeyCode::Escape);
		KEY_CASE(GLFW_KEY_ENTER,		KeyCode::Enter);
		KEY_CASE(GLFW_KEY_TAB,			KeyCode::Tab);
		KEY_CASE(GLFW_KEY_BACKSPACE,	KeyCode::Backspase);
		KEY_CASE(GLFW_KEY_INSERT,		KeyCode::Insert);
		KEY_CASE(GLFW_KEY_DELETE,		KeyCode::Delete);
		KEY_CASE(GLFW_KEY_RIGHT,		KeyCode::Right);
		KEY_CASE(GLFW_KEY_LEFT,			KeyCode::Left);
		KEY_CASE(GLFW_KEY_DOWN,			KeyCode::Down);
		KEY_CASE(GLFW_KEY_UP,			KeyCode::Up);
		KEY_CASE(GLFW_KEY_PAGE_UP,		KeyCode::PageUp);
		KEY_CASE(GLFW_KEY_PAGE_DOWN,	KeyCode::PageDown);
		KEY_CASE(GLFW_KEY_HOME,			KeyCode::Home);
		KEY_CASE(GLFW_KEY_END,			KeyCode::End);
		KEY_CASE(GLFW_KEY_CAPS_LOCK,	KeyCode::CapsLock);
		KEY_CASE(GLFW_KEY_SCROLL_LOCK,	KeyCode::ScrollLock);
		KEY_CASE(GLFW_KEY_NUM_LOCK,		KeyCode::NumLock);
		KEY_CASE(GLFW_KEY_PRINT_SCREEN, KeyCode::PrintScreen);
		KEY_CASE(GLFW_KEY_PAUSE,		KeyCode::Pause);

		KEY_CASE(GLFW_KEY_F1,	KeyCode::F1);
		KEY_CASE(GLFW_KEY_F2,	KeyCode::F2);
		KEY_CASE(GLFW_KEY_F3,	KeyCode::F3);
		KEY_CASE(GLFW_KEY_F4,	KeyCode::F4);
		KEY_CASE(GLFW_KEY_F5,	KeyCode::F5);
		KEY_CASE(GLFW_KEY_F6,	KeyCode::F6);
		KEY_CASE(GLFW_KEY_F7,	KeyCode::F7);
		KEY_CASE(GLFW_KEY_F8,	KeyCode::F8);
		KEY_CASE(GLFW_KEY_F9,	KeyCode::F9);
		KEY_CASE(GLFW_KEY_F10,	KeyCode::F10);
		KEY_CASE(GLFW_KEY_F11,	KeyCode::F11);
		KEY_CASE(GLFW_KEY_F12,	KeyCode::F12);
		KEY_CASE(GLFW_KEY_F13,	KeyCode::F13);
		KEY_CASE(GLFW_KEY_F14,	KeyCode::F14);
		KEY_CASE(GLFW_KEY_F15,	KeyCode::F15);
		KEY_CASE(GLFW_KEY_F16,	KeyCode::F16);
		KEY_CASE(GLFW_KEY_F17,	KeyCode::F17);
		KEY_CASE(GLFW_KEY_F18,	KeyCode::F18);
		KEY_CASE(GLFW_KEY_F19,	KeyCode::F19);
		KEY_CASE(GLFW_KEY_F20,	KeyCode::F20);
		KEY_CASE(GLFW_KEY_F21,	KeyCode::F21);
		KEY_CASE(GLFW_KEY_F22,	KeyCode::F22);
		KEY_CASE(GLFW_KEY_F23,	KeyCode::F23);
		KEY_CASE(GLFW_KEY_F24,	KeyCode::F24);
		KEY_CASE(GLFW_KEY_F25,	KeyCode::F25);

		KEY_CASE(GLFW_KEY_KP_0,			KeyCode::KP0);
		KEY_CASE(GLFW_KEY_KP_1,			KeyCode::KP1);
		KEY_CASE(GLFW_KEY_KP_2,			KeyCode::KP2);
		KEY_CASE(GLFW_KEY_KP_3,			KeyCode::KP3);
		KEY_CASE(GLFW_KEY_KP_4,			KeyCode::KP4);
		KEY_CASE(GLFW_KEY_KP_5,			KeyCode::KP5);
		KEY_CASE(GLFW_KEY_KP_6,			KeyCode::KP6);
		KEY_CASE(GLFW_KEY_KP_7,			KeyCode::KP7);
		KEY_CASE(GLFW_KEY_KP_8,			KeyCode::KP8);
		KEY_CASE(GLFW_KEY_KP_9,			KeyCode::KP9);
		KEY_CASE(GLFW_KEY_KP_DECIMAL,	KeyCode::KPDecimal);
		KEY_CASE(GLFW_KEY_KP_DIVIDE,	KeyCode::KPDivide);
		KEY_CASE(GLFW_KEY_KP_MULTIPLY,	KeyCode::KPMultiply);
		KEY_CASE(GLFW_KEY_KP_SUBTRACT,	KeyCode::KPSubtract);
		KEY_CASE(GLFW_KEY_KP_ADD,		KeyCode::KPAdd);
		KEY_CASE(GLFW_KEY_KP_ENTER,		KeyCode::KPEnter);
		KEY_CASE(GLFW_KEY_KP_EQUAL,		KeyCode::KPEqual);

		KEY_CASE(GLFW_KEY_LEFT_SHIFT,		KeyCode::LeftShift);
		KEY_CASE(GLFW_KEY_LEFT_CONTROL,		KeyCode::LeftControl);
		KEY_CASE(GLFW_KEY_LEFT_ALT,			KeyCode::LeftAlt);
		KEY_CASE(GLFW_KEY_LEFT_SUPER,		KeyCode::LeftSuper);
		KEY_CASE(GLFW_KEY_RIGHT_SHIFT,		KeyCode::Escape);
		KEY_CASE(GLFW_KEY_RIGHT_CONTROL,	KeyCode::Escape);
		KEY_CASE(GLFW_KEY_RIGHT_ALT,		KeyCode::Escape);
		KEY_CASE(GLFW_KEY_RIGHT_SUPER,		KeyCode::Escape);
		KEY_CASE(GLFW_KEY_MENU,				KeyCode::Escape);
				
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
	case GLFW_MOUSE_BUTTON_LEFT:	Varlet::Core::Input::AddMouse(MouseCode::Left, state);		break;
	case GLFW_MOUSE_BUTTON_RIGHT:	Varlet::Core::Input::AddMouse(MouseCode::Right, state);		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:	Varlet::Core::Input::AddMouse(MouseCode::Middle, state);	break;
	}
}

static void WindowCloseCallback(GLFWwindow* window)
{
	Varlet::Core::Engine::WantExit();
}

static void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	Varlet::Core::ScreenInternal::SetResolution(width, height);
}

namespace Varlet::Core
{
	HMODULE Platform::_module;

	void Platform::Initialize()
	{
		std::locale::global(std::locale(""));

		_module = GetModuleHandle(nullptr);
	}

	void Platform::Update()
	{
		glfwPollEvents();
		TimeInternal::UpdateTime(glfwGetTime());
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	//PlatformNativeWindow Platform::CreateWindowNative(const wchar_t* title)
	//{
	//	WNDCLASS wc = { 0 };
	//	wc.lpfnWndProc = WndProc;
	//	wc.hInstance = _module;
	//	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	//	wc.lpszClassName = title;
	//	wc.style = CS_OWNDC;
	//	
	//	if (!RegisterClass(&wc))
	//		return nullptr;
	//	
	//	return CreateWindowW(wc.lpszClassName, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 640, 480, 0, 0, _module, 0);
	//}

	Platform::WindowDescriptor Platform::CreateWindow(const char* title)
	{
		static bool wasCreated = false;

		if (glfwInit() == GLFW_TRUE)
			wasCreated = true;
		
		const auto window = glfwCreateWindow(1280, 720, title, nullptr, nullptr);
		assert(window);

		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);

		if (Input::IsSupportKeyboard())
			glfwSetKeyCallback(window, KeyCallback);

		if (Input::IsSupportMouse())
		{
			glfwSetCursorPosCallback(window, CursorPosCallback);
			glfwSetMouseButtonCallback(window, MouseButtonCallback);
		}

		glfwSetFramebufferSizeCallback(window, WindowResizeCallback);
		glfwSetWindowCloseCallback(window, WindowCloseCallback);

		ScreenInternal::Initialize(window);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		ScreenInternal::SetResolution(width, height);
		
		return window;
	}

	void Platform::DestroyWindow(WindowDescriptor window)
	{
		glfwDestroyWindow(window);
	}

	Platform::Module Platform::LoadModule(const wchar_t* name)
	{
		Platform::Module module = nullptr;

		try
		{
			module = LoadLibrary(name);
		}
		catch (const std::exception& e)
		{
			VARLET_LOG(Error, WIDE("Load module exception"));
		}

		CHECK_PLATFORM_ERROR;

		if (module == nullptr)
		{
			VARLET_LOG(Error, WIDE("Loading module \"%s\" was failed"), name);
		}

		return module;
	}

	void* Platform::LoadFunction(Platform::Module& module, const char* name)
	{
		void* function = GetProcAddress(module, name);
		
		CHECK_PLATFORM_ERROR;

		if (function == nullptr)
			VARLET_LOG(Error, WIDE("Failed to load %s() function"), name);

		return function;
	}

	bool Platform::FreeModule(Platform::Module& module)
	{
		return FreeLibrary(module);
	}

	std::wstring Platform::ConvertToWide(const char* string)
	{
		const UINT codePage = GetACP();
		
		const int newLength = MultiByteToWideChar(codePage, 0, string, -1, nullptr, 0);

		std::wstring convertedString;
		convertedString.resize(newLength - 1);

		MultiByteToWideChar(codePage, 0, string, -1, &convertedString[0], newLength);

		return convertedString;
	}

	std::string Platform::ConvertFromWide(const wchar_t* string)
	{
		const UINT codePage = GetACP();

		const int newLength = WideCharToMultiByte(codePage, 0, string, -1, nullptr, 0, nullptr, nullptr);

		std::string convertedString;
		convertedString.resize(newLength - 1);

		WideCharToMultiByte(codePage, 0, string, -1, &convertedString[0], newLength, nullptr, nullptr);

		return convertedString;
	}

	void Platform::SetConsoleColor(WORD color)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}

	std::wstring Platform::CreateFileDialog(const wchar_t* filter)
	{
		OPENFILENAME ofn;
		WCHAR szFile[260] = { 0 };
		WCHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Screen::GetNative());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectory(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileName(&ofn) == TRUE)
			return ofn.lpstrFile;

		return WIDE("");
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