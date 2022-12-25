#pragma once

#include <Windows.h>

#ifdef CreateWindow
	#undef CreateWindow
#endif

#ifdef max 
	#undef max
#endif

#ifdef min
	#undef min
#endif

struct GLFWwindow;

namespace Varlet
{
	class Platform final
	{
	public:

		using WindowDescriptor = GLFWwindow*;

	private:

		static HMODULE _module;

	public:

		static void Init();

		static void Update();

		static void CreateContext();

	private:

		
	};

	class PlatformGL final
	{
	public:

		static void MakeCurrent(const Platform::WindowDescriptor context);

		static void SwapBuffers(const Platform::WindowDescriptor context);
	};
}
