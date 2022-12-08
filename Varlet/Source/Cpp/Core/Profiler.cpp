#include "Profiler.h"

namespace Varlet
{
	Profiler::Profiler(const char* areaName)
		: _start(std::chrono::steady_clock::now()),
		_areaName(areaName),
		_outResult(nullptr)
	{
	}

	Profiler::Profiler(double* outResult)
		: _start(std::chrono::steady_clock::now()),
		_areaName(),
		_outResult(outResult)
	{
	}

	Profiler::~Profiler()
	{
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - _start;

		if (_outResult)
			*_outResult = elapsed.count();
	}
}
