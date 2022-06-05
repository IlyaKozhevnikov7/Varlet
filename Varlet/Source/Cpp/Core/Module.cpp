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

	void Module::Init()
	{
	}

	void Module::InitDependencies(std::vector<Module*>& dependencies)
	{
	}
}
