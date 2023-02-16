#pragma once

#include "Common.h"
#include <chrono>

#ifdef VARLET_DEBUG
	#define PROFILE_OUT(OutResult) Varlet::Core::Profiler timer(&OutResult);
	#define PROFILE_NAMED(Name) Varlet::Core::Profiler timer(Name);
	#define PROFILE() Varlet::Core::Profiler timer(__FUNCTION__);
#else
	#define PROFILE_OUT(OutResult)
	#define PROFILE_NAMED(Name)
	#define PROFILE()
#endif // VARLET_DEBUG

namespace Varlet::Core
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
