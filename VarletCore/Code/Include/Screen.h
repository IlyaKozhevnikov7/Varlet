#pragma once

#include "Common.h"
#include "Platform/Current.h"

class CORE_API Screen final
{
public:

	static glm::ivec2 GetResolution();

	static Varlet::Core::Platform::WindowDescriptor GetNative();
};

namespace Varlet::Core
{
	class ScreenInternal final
	{
		friend class Screen;

	private:

		static Platform::WindowDescriptor _window;
		static int32_t _width;
		static int32_t _height;

	public:

		static void Initialize(Platform::WindowDescriptor window);

		static void SetResolution(int32_t width, int32_t height);
	};
}
