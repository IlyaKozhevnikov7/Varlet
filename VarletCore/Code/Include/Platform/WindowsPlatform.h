#pragma once

#include "Common.h"
#include <Windows.h>

#undef CreateWindow
#undef max
#undef min
#undef FreeModule

struct GLFWwindow;

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

		static void Init();

		static void Update();

		CORE_API static void CreateWindow();

		CORE_API static Platform::Module LoadModule(const char* name);

		CORE_API static void* LoadFunction(Platform::Module& module, const char* name);

		CORE_API static bool FreeModule(Platform::Module& module);
	};

	class CORE_API PlatformGL final
	{
	public:

		static void MakeCurrent(const Platform::WindowDescriptor context);

		static void SwapBuffers(const Platform::WindowDescriptor context);
	};
}
