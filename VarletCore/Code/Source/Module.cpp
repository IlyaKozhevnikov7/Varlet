#include "Module.h"
#include "Reflection.h"

namespace Varlet::Core
{
	std::vector<const char*> Module::GetDependencies() const
	{
		return { };
	}

	bool Module::Init()
	{
		return true;
	}

	bool Module::PostInit()
	{
		return true;
	}

	void Module::Shutdown()
	{
	}

	std::vector<const Type*>& Module::GetTypes()
	{
		static std::vector<const Type*> empty;
		return empty;
	}
}
