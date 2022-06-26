#include "Engine.h"
#include "GLFWContext.h"
#include "OpenGLRenderer.h"

namespace Varlet
{
	Engine::Engine()
	{
		_context = new GLFWContext(); // hardcode context
		_isRunning = true;
	}

	Engine::~Engine()
	{
		delete _context;
	}

	Engine* Engine::Get()
	{
		static Engine instance;
		return &instance;
	}

	ContextAPI* Engine::GetContext() const
	{
		return _context;
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

		// temp add renderer here
		{
			auto openGLRenderer = new OpenGLRenderer();
			_modules.push_back(openGLRenderer);
			_updatebleModules.push_back(openGLRenderer);
		}
	}

	void Engine::InitModules()
	{
		for (auto module : _modules)
			if (module->Init() == FAILED_INITIALIZATION)
			{
				VARLET_LOG(LevelType::Error, "Module initialization error");
			}

		for (auto module : _modules)
			if (module->PostInit() == FAILED_INITIALIZATION)
			{
				VARLET_LOG(LevelType::Error, "Module post initialization error");
			}
	}

	void Engine::Run()
	{
		while (IsRunning())
		{
			_context->Update();

			for (auto module : _updatebleModules)
				module->Update();
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
