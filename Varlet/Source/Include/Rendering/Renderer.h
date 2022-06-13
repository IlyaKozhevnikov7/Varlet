#pragma once

#include "VarletCore.h"
#include "Module.h"

namespace Varlet
{
	class Renderer : public Module, public IUpdatebleModule
	{
	public:

		~Renderer() override = default;
	};
}
