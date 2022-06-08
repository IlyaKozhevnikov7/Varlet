#include "Engine.h"
#include "GLFWContext.h"

namespace Varlet
{
	Engine::Engine()
	{
		_context = new GLFWContext();
		_isRunning = true;
	}

	Engine::~Engine()
	{
		delete _context;
	}

	void Engine::Init()
	{
		_context->Init();
	}

	void Engine::RegisterTargetModule(Module* target)
	{
		std::vector<Module*> modules;
		modules.push_back(target);

		std::vector<Module*>&& dependencies = std::move(target->GetDependencies());
		if (dependencies.size() > 0)
			modules.insert(modules.begin(), dependencies.begin(), dependencies.end());

		for (auto module : modules)
		{
			_modules.push_back(module);

			if (auto updatebleModule = dynamic_cast<IUpdatebleModule*>(module))
				_updatebleModules.push_back(updatebleModule);
		}
	}

	void Engine::InitModules()
	{
		for (auto module : _modules)
			module->Init();
	}

	void Engine::Run()
	{
		while (IsRunning())
		{
			for (auto module : _updatebleModules)
				module->Update();

			_context->Update();
		}
	}

	void Engine::Shutdown()
	{
		_context->Shutdown();

		for (auto module : _modules)
			module->Shutdown();
	}

	bool Engine::IsRunning() const
	{
		return _isRunning && _context->IsRunning();
	}
}
