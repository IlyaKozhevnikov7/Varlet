#include "Screen.h"

glm::ivec2 Screen::GetResolution()
{
	return { Varlet::Core::ScreenInternal::_width, Varlet::Core::ScreenInternal::_height };
}

Varlet::Core::Platform::WindowDescriptor Screen::GetNative()
{
	return Varlet::Core::ScreenInternal::_window;
}

namespace Varlet::Core
{
	Platform::WindowDescriptor ScreenInternal::_window;
	int32_t ScreenInternal::_width;
	int32_t ScreenInternal::_height;

	void ScreenInternal::Initialize(Platform::WindowDescriptor window)
	{
		_window = window;
	}

	void ScreenInternal::SetResolution(int32_t width, int32_t height)
	{
		_width = width;
		_height = height;
	}
}
