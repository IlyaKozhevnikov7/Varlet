#pragma once

#include "VarletCore.h"
#include "Module.h"

namespace Varlet
{
	class Renderer : public Module, public IUpdatebleModule
	{
	public:

		virtual int32_t Init() override;

		~Renderer() override = default;
	};
}
