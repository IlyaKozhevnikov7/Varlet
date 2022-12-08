#include "VarletTime.h"

Varlet::TimeModule* Time::_core;

bool Time::Init(Varlet::TimeModule* core)
{
	_core = core;

	return _core != nullptr ? SUCCESSFUL_INITIALIZATION : FAILED_INITIALIZATION;
}

float Time::GetTime()
{
	return _core->GetTime();
}

float Time::GetDeltaTime()
{
	return _core->GetDeltaTime();
}
