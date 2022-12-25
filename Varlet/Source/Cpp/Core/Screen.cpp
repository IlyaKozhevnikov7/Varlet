#include "Screen.h"

glm::ivec2 Screen::GetResolution()
{
	return { Varlet::ScreenInternal::_width, Varlet::ScreenInternal::_height };
}

void* Screen::GetNative()
{
	return Varlet::ScreenInternal::_window;
}

namespace Varlet
{
	Platform::WindowDescriptor ScreenInternal::_window;
	int32_t ScreenInternal::_width;
	int32_t ScreenInternal::_height;

	void ScreenInternal::Init(Platform::WindowDescriptor window)
	{
		_window = window;
	}

	void ScreenInternal::SetResolution(const int32_t width, const int32_t height)
	{
		_width = width;
		_height = height;
	}
}
