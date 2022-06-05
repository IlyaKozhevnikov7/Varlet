#include "Module.h"

namespace Varlet
{
	std::vector<Module*>& Module::GetDependencies() const
	{
		return _dependencies;
	}

	void Module::Init()
	{
		InitDependencies(_dependencies);
	}

	void Module::InitDependencies(std::vector<Module*>& dependencies)
	{
	}
}
