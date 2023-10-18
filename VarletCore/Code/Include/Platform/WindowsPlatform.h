#pragma once

#include "Common.h"
#include <Windows.h>

#undef CreateWindow
#undef max
#undef min
#undef FreeModule

#define CC_GRAY 7
#define CC_YELLOW 14
#define CC_RED 12

struct GLFWwindow;
typedef HWND PlatformNativeWindow;

namespace Varlet::Core
{
	class Platform final
	{
	public:

		using Module = HMODULE;
		using WindowDescriptor = GLFWwindow*;

	private:

		static HMODULE _module;

	public:

		static void Initialize();

		static void Update();

		//CORE_API static PlatformNativeWindow CreateWindowNative(const wchar_t* title);

		CORE_API static WindowDescriptor CreateWindow(const char* title);

		CORE_API static void DestroyWindow(WindowDescriptor window);

		CORE_API static Platform::Module LoadModule(const wchar_t* name);

		CORE_API static void* LoadFunction(Platform::Module& module, const char* name);

		CORE_API static bool FreeModule(Platform::Module& module);

		CORE_API static std::wstring ConvertToWide(const char* string);

		CORE_API static std::string ConvertFromWide(const wchar_t* string);

		CORE_API static void SetConsoleColor(WORD color);

		CORE_API static std::wstring CreateFileDialog(const wchar_t* filter);
	};

	class CORE_API PlatformGL final
	{
	public:

		static void MakeCurrent(const Platform::WindowDescriptor context);

		static void SwapBuffers(const Platform::WindowDescriptor context);
	};
}
