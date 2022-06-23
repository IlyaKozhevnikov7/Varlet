#include "Module.h"

namespace Varlet
{
	std::vector<Module*>& Module::GetDependencies() const
	{
		return _dependencies;
	}

	int32_t Module::Init()
	{
		InitDependencies(_dependencies);

		return SUCCESSFUL_INITIALIZATION;
	}

	void Module::Shutdown()
	{
	}

	void Module::InitDependencies(std::vector<Module*>& dependencies)
	{
	}

	int32_t GameModule::Init()
	{
		Module::Init();

		// load startup scene

		VARLET_LOG(LevelType::Normal, "GameModule init");
		return SUCCESSFUL_INITIALIZATION;
	}

	void GameModule::Update()
	{
		// update scene (game loop)
	}
}
