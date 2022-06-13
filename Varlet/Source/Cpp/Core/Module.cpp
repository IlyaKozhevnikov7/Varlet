#include "Module.h"

namespace Varlet
{
	Module::Module()
	{
		InitDependencies(_dependencies);
	}

	std::vector<Module*>& Module::GetDependencies() const
	{
		return _dependencies;
	}

	int32_t Module::Init()
	{
		return SUCCESSFUL_INITIALIZATION;
	}

	void Module::Shutdown()
	{
	}

	void Module::InitDependencies(std::vector<Module*>& dependencies)
	{
	}
}
