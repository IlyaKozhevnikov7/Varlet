#pragma once

#include "VarletCore.h"
#include "Platform/Current.h"

class CORE_API Screen final
{
public:

	static glm::ivec2 GetResolution();

	static void* GetNative();
};


namespace Varlet
{
	class ScreenInternal final
	{
		friend class Screen;

	private:

		static Varlet::Platform::WindowDescriptor _window;
		static int32_t _width;
		static int32_t _height;

	public:

		static void Init(Varlet::Platform::WindowDescriptor window);

		static void SetResolution(const int32_t width, const int32_t height);
	};
}
