#pragma once

#include "VarletCore.h"
#include "Module.h"

namespace Varlet
{
	class Renderer : public Module, public IUpdatebleModule
	{
	public:

<<<<<<< HEAD
		virtual int32_t GetAPIId() const = 0;
=======
		virtual int32_t Init() override;
>>>>>>> dev

		~Renderer() override = default;
	};
}
