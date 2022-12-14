#pragma once

#include "VarletCore.h"
#include "TimeModule.h"

namespace Varlet
{
	class TimeModule;
}

class Time final
{
private:

	static Varlet::TimeModule* _core;

public:

	static bool Init(Varlet::TimeModule* core);

	CORE_API static float GetTime();

	CORE_API static float GetDeltaTime();
};

