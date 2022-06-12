#pragma once

#include "VarletCore.h"
#include "Module.h"

namespace Varlet
{
	class CORE_API Renderer final : public Module, public IUpdatebleModule
	{
	public:

		virtual void Draw() = 0;
	};
}
