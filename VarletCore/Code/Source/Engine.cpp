#include "Engine.h"
#include "Platform/Current.h"
#include "ModuleService.h"

namespace Varlet::Core
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

	void Engine::Initialize(const char* projcetDirectory)
	{
#ifdef VARLET_EDITOR
		auto target = dynamic_cast<EditorModule*>(ModuleService::LoadModule("Editor"));

		if (target != nullptr)
			target->projectDirectory = projcetDirectory;
#else

#endif // VARLET_EDITOR
	}

	void Engine::InitModules()
	{
		for (auto module : _modules)
			if (module->Init() == false)
				VARLET_LOG(LevelType::Error, "Module initialization error");

		for (auto module : _modules)
			if (module->PostInit() == false)
				VARLET_LOG(LevelType::Error, "Module post initialization error");
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
			module->Shutdown();

		ModuleService::FreeModules();
	}

	void Engine::WantExit()
	{
		Get()->_isRunning = false;
	}

	bool Engine::IsRunning() const
	{
		return _isRunning;
	}

	void Engine::AddModule(Module* module)
	{
		_modules.push_back(module);

		if (auto updatable = dynamic_cast<IUpdatebleModule*>(module))
			_updatebleModules.push_back(updatable);

		for (auto& dependence : module->GetDependencies())
			ModuleService::LoadModule(dependence);
	}

	void Engine::RemoveModule(Module* module)
	{
		_modules.erase(std::remove(_modules.begin(), _modules.end(), module), _modules.end());

		if (auto updatable = dynamic_cast<IUpdatebleModule*>(module))
			_updatebleModules.erase(std::remove(_updatebleModules.begin(), _updatebleModules.end(), updatable), _updatebleModules.end());
	}
}
