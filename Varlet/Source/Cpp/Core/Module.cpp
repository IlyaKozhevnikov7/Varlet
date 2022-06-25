#include "Module.h"

#include "Scene.h"

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

	int32_t Module::PostInit()
	{
		return SUCCESSFUL_INITIALIZATION;
	}

	void Module::Shutdown()
	{
	}

	void Module::InitDependencies(std::vector<Module*>& dependencies)
	{
	}

	Scene* GameModule::GetCurrentScene() const
	{
		return _currentScene;
	}

	int32_t GameModule::Init()
	{
		Module::Init();

		// load startup scene

		// TODO remove scene creation
		_currentScene = new Scene();

		VARLET_LOG(LevelType::Normal, "GameModule init");
		return SUCCESSFUL_INITIALIZATION;
	}

	void GameModule::Update()
	{
		// update scene (game loop)
	}
}
