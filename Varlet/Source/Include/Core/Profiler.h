#pragma once

#include "VarletCore.h"
#include <chrono>

#ifdef DEBUG
	#define PROFILE_OUT(OutResult) Varlet::Profiler timer(&OutResult);
	#define PROFILE_NAMED(Name) Varlet::Profiler timer(Name);
	#define PROFILE() Varlet::Profiler timer(__FUNCTION__);
#else
	#define PROFILE_OUT(OutResult)
	#define PROFILE_NAMED(Name)
	#define PROFILE()
#endif // DEBUG

namespace Varlet
{
	struct CORE_API Profiler final
	{
	private:

		using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

		TimePoint _start;
		const char* _areaName;
		double* _outResult;

	public:

		Profiler(const char* areaName);

		Profiler(double* outResult);

		~Profiler();
	};
}
