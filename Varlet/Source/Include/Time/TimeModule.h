#pragma once

#include "VarletCore.h"
#include "VarletModules.h"

namespace Varlet
{
	class CORE_API TimeModule : public Module
	{
	public:

		virtual int32_t Init() override;

		virtual void UpdateTime() = 0;

		virtual float GetTime() = 0;

		virtual float GetDeltaTime() = 0;
	};
}
