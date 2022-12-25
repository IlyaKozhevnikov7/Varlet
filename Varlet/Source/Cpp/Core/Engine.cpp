#include "Engine.h"
#include "Platform/Current.h"
#include "OpenGL/Graphics.h"
#include "Windows/GLFWTime.h"

#ifdef DEBUG
	#include "Debug.h"
#endif // DEBUG

namespace Varlet
{
	Engine::Engine()
	{
		Platform::Init();

		_isRunning = true;
	}

	Engine* Engine::Get()
	{
		static Engine instance;
		return &instance;
	}

	void Engine::RegisterTargetModule(Module* target)
	{
		std::vector<Module*> modules;

		std::vector<Module*> dependencies = std::move(target->GetDependencies());
		modules.push_back(target);

		if (dependencies.size() > 0)
			modules.insert(modules.begin(), dependencies.begin(), dependencies.end());

		// temp add input and time here
		{
			auto time = new GLFWTime();
			_modules.push_back(time);
			_updatebleModules.push_back(time);
		}

		for (auto module : modules)
		{
			_modules.push_back(module);

			if (auto updatebleModule = dynamic_cast<IUpdatebleModule*>(module))
				_updatebleModules.push_back(updatebleModule);
		}

		// temp add renderer here
		{
			auto openGLGraphics = new OpenGL::Graphics();
			_modules.push_back(openGLGraphics);
			_updatebleModules.push_back(openGLGraphics);
		}

		{
#ifdef DEBUG
			const auto debug = new DebugModule();
			_modules.push_back(debug);
			_updatebleModules.push_back(debug);
#endif // DEBUG
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
			Platform::Update();

			for (auto module : _updatebleModules)
				module->Update();
		}
	}

	void Engine::Shutdown()
	{
		for (auto module : _modules)
		{
			module->Shutdown();
			delete module;
		}
	}

	void Engine::WantExit()
	{
		Get()->_isRunning = false;
	}

	bool Engine::IsRunning() const
	{
		return _isRunning;
	}
}
