#include "Module.h"

namespace Varlet
{
	void Module::Init()
	{
		InitDependencies(_dependencies);
	}

	void Module::InitDependencies(const std::vector<Module*>& dependencies)
	{
	}
}
