#pragma once

#include "VarletCore.h"
#include "VarletModules.h"

namespace Varlet
{
	class CORE_API TimeModule : public Module, public IUpdatebleModule
	{
	public:

		~TimeModule() override = default;

		virtual int32_t Init() override;

		virtual float GetTime() = 0;

		virtual float GetDeltaTime() = 0;
	};
}
