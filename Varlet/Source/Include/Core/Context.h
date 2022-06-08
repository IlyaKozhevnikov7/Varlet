#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class ContextAPI
	{
	public:

		virtual ~ContextAPI() = default;

		virtual int32_t Init() = 0;

		virtual void Update() = 0;

		virtual void Shutdown() = 0;
	};

	template<typename TWindow>
	class Context : public ContextAPI
	{
	protected:

		TWindow* _window;

	public:

		~Context() override
		{
			delete _window;
		}
	};
}

