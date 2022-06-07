#pragma once

#include "Varlet.h"

namespace Varlet
{
	template<class TWindow>
	class Context
	{
	private:

		TWindow* _window;

	public:

		Context() = default;
		~Context()
		{
			delete _window
		}

		virtual int32_t Init() = 0;
		virtual void Shutdown() = 0;
	};
}

