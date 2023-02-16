#include "CoreTime.h"

float Time::GetTime()
{
	return Varlet::Core::TimeInternal::_time;
}

float Time::GetDeltaTime()
{
	return Varlet::Core::TimeInternal::_deltaTime;
}

namespace Varlet::Core
{
	float TimeInternal::_time;
	float TimeInternal::_deltaTime;
	float TimeInternal::_lastUpdateTime;

	void TimeInternal::UpdateTime(const float& time)
	{
		_deltaTime = time - _lastUpdateTime;
		_lastUpdateTime = _time;
		_time = time;
	}
}
