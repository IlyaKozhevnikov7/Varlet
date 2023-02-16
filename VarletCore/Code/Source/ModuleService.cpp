#include "ModuleService.h"
#include "Module.h"
#include "Engine.h"

namespace Varlet::Core
{
	std::unordered_map<std::string, ModuleService::ModuleInfo> ModuleService::_registered;

	Module* ModuleService::LoadModule(const char* path, const bool& withEngineRegistration)
	{
		using OnLoadFunctionSignature = Module*(*)();

		if (_registered.contains(path))
			return nullptr;

		auto platformModule = Platform::LoadModule(path);
		if (platformModule == nullptr)
		{
			// TODO log
			return nullptr;
		}

		auto loadFunction = reinterpret_cast<OnLoadFunctionSignature>(Platform::LoadFunction(platformModule, "OnLoad"));
		if (loadFunction == nullptr)
		{
			// TODO log
			return nullptr;
		}

		auto module = loadFunction();
		if (module == nullptr)
		{
			// TODO log
			return nullptr;
		}

		if (withEngineRegistration)
			Engine::Get()->AddModule(module);

		_registered[path] = { platformModule, module };
		return module;
	}

	bool ModuleService::FreeModule(const char* path)
	{
		using OnFreeFunctionSignature = void(*)();

		if (_registered.contains(path))
		{
			auto[platformModule, module] = _registered[path]; 

			auto freeFunction = reinterpret_cast<OnFreeFunctionSignature>(Platform::LoadFunction(platformModule, "OnFree"));
			
			if (freeFunction != nullptr)
			{
				freeFunction();
			}
			else
			{
				// TODO log
			}

			Engine::Get()->RemoveModule(module);

			const bool result = Platform::FreeModule(platformModule);
			_registered.erase(path);
			return result;
		}

		return false;
	}

	void ModuleService::FreeModules()
	{
		//std::vector<ModuleInfo> moduleInfos;
		//
		//for (auto info : _registered)
		//	moduleInfos.push_back(info.second);
		//
		//for (auto info : moduleInfos)
		//	ModuleService::FreeModule(info.platformModule);

		//_registered.clear();
	}
}
