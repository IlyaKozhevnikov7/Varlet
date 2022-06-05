#include "Engine.h"

namespace Varlet
{
	void Engine::Init()
	{
	}

	void Engine::RegisterTargetModule(Module* target)
	{
		target->Init();

		std::vector<Module*> modules;
		modules.push_back(target);

		std::vector<Module*>&& dependencies = std::move(target->GetDependencies());
		if (dependencies.size() > 0)
			modules.insert(modules.begin(), dependencies.begin(), dependencies.end());

		for (auto& module : modules)
		{
			_modules.push_back(module);

			if (auto updatebleModule = dynamic_cast<IUpdatebleModule*>(module))
				_updatebleModules.push_back(updatebleModule);
		}
	}

	void Engine::Run()
	{
		for (int i = 0; i < 5; i++)
			for (auto module : _updatebleModules)
				module->Update();
	}

	void Engine::Shutdown()
	{
		for (auto module : _modules)
			module->Shutdown();
	}
}
