#pragma once

#include "Common.h"

class CORE_API Time final
{
public:

	static float GetTime();

	static float GetDeltaTime();
};

namespace Varlet::Core
{
	class TimeInternal final
	{
		friend Time;

	private:

		static float _time;
		static float _deltaTime;
		static float _lastUpdateTime;

	public:

		static void UpdateTime(const float& time);
	};
}

