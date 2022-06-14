#pragma once

#include "VarletCore.h"
#include "Engine.h"

class CORE_API VarletAPI
{
public:

	template<typename TWindow>
	static TWindow* GetContextWindow()
	{
		return static_cast<TWindow*>(Varlet::Engine::Get()->GetContext()->GetWindow());
	}
};

