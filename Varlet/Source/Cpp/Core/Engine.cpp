#include "Engine.h"
#include "GLFWContext.h"
#include "GLFWInput.h"
#include "OpenGLRenderer.h"
#include "GLFWTime.h"

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

		std::vector<Module*> dependencies = std::move(target->GetDependencies());
		modules.push_back(target);

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

		// temp add input and time here
		{
			_modules.push_back(new GLFWInput());
			_modules.push_back(new GLFWTime());
		}
	}

	void Engine::InitModules()
	{
		for (auto module : _modules)
		{
			if (module->Init() == FAILED_INITIALIZATION)
			{
				VARLET_LOG(LevelType::Error, "Module initialization error");
			}
		}

		for (auto module : _modules)
		{
			if (module->PostInit() == FAILED_INITIALIZATION)
			{
				VARLET_LOG(LevelType::Error, "Module post initialization error");
			}
		}
	}

	void Engine::Run()
	{
		while (IsRunning())
		{
			Time::UpdateTime();

			_context->Update();

			for (auto module : _updatebleModules)
				module->Update();
		}
	}

	void Engine::Shutdown()
	{
		_context->Shutdown();

		for (auto module : _modules)
		{
			module->Shutdown();
			delete module;
		}
	}

	bool Engine::IsRunning() const
	{
		return _isRunning && _context->IsRunning();
	}
}