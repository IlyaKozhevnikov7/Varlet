#include "Engine.h"
#include "Platform/Current.h"
#include "ModuleService.h"
#include "Paths.h"

// TODO: relocate
#include "Reflection/Type.h"
#include "Serialization/SerializationUtils.h"

namespace Varlet::Core
{
	Engine Engine::_instance;

	Engine::Engine()
	{
		Platform::Initialize();

		_isRunning = true;
	}

	Engine* Engine::Get()
	{
		return &_instance;
	}

	void Engine::Initialize(const std::wstring& launcherPath, const std::wstring& projcetDirectory)
	{
		_updatebleModules.reserve(10);

		ModuleService::OnRegisteredNewModule.Bind([](const ModuleInfo& info)
			{
				VT_LOG(Normal, "Module %s has been loaded", info.modulePath.c_str());
			});
		
		ModuleService::OnUnregisteredNewModule.Bind([](const ModuleInfo& info)
			{
				VT_LOG(Normal, "Module %s has been unloaded", info.modulePath.c_str());
			});

		Varlet::Core::Type::Register(nullptr, ICustomSerializable::GetTypeStatic());

		Varlet::Core::Paths::InitializeEnginePath(launcherPath.c_str());
		Varlet::Core::Paths::InitializeApplicationPath(projcetDirectory.c_str());

#ifdef VARLET_DEVELOPMENT
		ModuleService::LoadModule(WIDE("Editor"));
#else
		ModuleService::LoadModule(projcetDirectory);
#endif // VARLET_DEVELOPMENT
	}

	void Engine::PostInitializeModules()
	{
		for (auto module : _modules)
			if (module->PostInitialize() == false)
				VARLET_LOG(Error, WIDE("Module post-initialization error"));
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
	}

	void Engine::RemoveModule(Module* module)
	{
		_modules.erase(std::remove(_modules.begin(), _modules.end(), module), _modules.end());

		if (auto updatable = dynamic_cast<IUpdatebleModule*>(module))
			_updatebleModules.erase(std::remove(_updatebleModules.begin(), _updatebleModules.end(), updatable), _updatebleModules.end());

		module->Shutdown();
	}
}
