#include "ModuleService.h"
#include "Module.h"
#include "Engine.h"
#include "Paths.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace Varlet::Core
{
	Event<const ModuleInfo&> ModuleService::OnRegisteredNewModule;
	Event<const ModuleInfo&> ModuleService::OnUnregisteredNewModule;

	std::vector<ModuleInfo> ModuleService::_registered;

	Module* ModuleService::LoadModule(const wchar_t* modulePath, bool registeredByEngine)
	{
#ifdef VARLET_DEVELOPMENT
		return LoadModuleInternal(modulePath, registeredByEngine)->module;
#else
		// TODO: implement for release version
		return nullptr;
#endif
	}

	bool ModuleService::FreeModule(Platform::Module platformModule)
	{
		return FreeModuleInternal(platformModule, true);
	}

	bool ModuleService::FreeModuleLibrary(Platform::Module platformModule)
	{
		using OnFreeFunctionSignature = void(*)();

		auto freeFunction = std::bit_cast<OnFreeFunctionSignature>(Platform::LoadFunction(platformModule, "OnFree"));

		if (freeFunction != nullptr)
		{
			freeFunction();
		}
		else
		{
			// TODO log
		}

		const auto it = std::remove_if(_registered.begin(), _registered.end(), [&platformModule](const ModuleInfo& info)
			{
				return info.platformModule == platformModule;
			});

		_registered.erase(it, _registered.end());

		return Platform::FreeModule(platformModule);
	}

	void ModuleService::FreeModules()
	{
		while (_registered.empty() != true)
			FreeModuleInternal(_registered.back().platformModule, false);
	}

	Platform::Module ModuleService::GetPlatformModule(Module* module)
	{
		auto it = std::find_if(_registered.begin(), _registered.end(), [&module](ModuleInfo& info)
			{
				return info.module == module;
			});

		return it != _registered.end()
			? (*it).platformModule
			: nullptr;
	}

	std::wstring ModuleService::GetModulePath(Module* module)
	{
		const auto it = std::find_if(_registered.begin(), _registered.end(), [&module](const ModuleInfo& info)
			{
				return info.module == module;
			});

		return it != _registered.end()
			? (*it).libraryPath
			: L"";
	}

	int32_t ModuleService::GetModuleIdByPath(const std::wstring_view path)
	{
		const auto it = std::find_if(_registered.begin(), _registered.end(), [&path](const ModuleInfo& info)
			{
				return info.modulePath == path;
			});

		return it != _registered.end()
			? std::distance(_registered.begin(), it)
			: -1;
	}

	std::wstring ModuleService::GetModulePathById(int32_t id)
	{
		if (id >= _registered.size())
			return WIDE("");

		return _registered[id].modulePath;
	}

	ModuleInfo* ModuleService::LoadModuleInternal(const wchar_t* modulePath, bool registeredByEngine)
	{
		const std::wstring path = GetModuleDirectoryPath(modulePath);
		int32_t index = IndexWithName(path);

		if (index >= 0)
			return &_registered[index];

		ModuleInfo moduleInfo;
		moduleInfo.modulePath = path;
		moduleInfo.libraryPath = GetModuleLibraryPath(path.c_str());

		LoadDependencies(moduleInfo);
		auto moduleDescriptors = LoadModuleFromLibrary(moduleInfo.libraryPath.c_str());

		moduleInfo.platformModule = moduleDescriptors.first;
		moduleInfo.module = moduleDescriptors.second;

		if (moduleInfo.module == nullptr)
			return nullptr;

		_registered.push_back(moduleInfo);

		if (registeredByEngine)
			Engine::Get()->AddModule(moduleInfo.module);

		moduleInfo.module->Initialize();
		
		OnRegisteredNewModule.Invoke(moduleInfo);

		index = GetModuleIdByPath(moduleInfo.modulePath);
		assert(index >= 0 && index < _registered.size());

		return &_registered[index];
	}

	std::pair<Platform::Module, Module*> ModuleService::LoadModuleFromLibrary(const wchar_t* path)
	{
		using OnLoadFunctionSignature = Module*(*)();

		auto platformModule = Platform::LoadModule(path);
		if (platformModule == nullptr)
		{
			// TODO log
			return {};
		}

		auto loadFunction = std::bit_cast<OnLoadFunctionSignature>(Platform::LoadFunction(platformModule, "OnLoad"));
		if (loadFunction == nullptr)
		{
			Platform::FreeModule(platformModule);
			// TODO log
			return {};
		}

		auto module = loadFunction();
		if (module == nullptr)
		{
			Platform::FreeModule(platformModule);

			// TODO log
			return {};
		}

		return { platformModule, module };
	}

	bool ModuleService::FreeModuleInternal(Platform::Module platformModule, bool callModuleShutdown)
	{
		for (auto& info : _registered)
			if (info.platformModule == platformModule)
			{
				if (callModuleShutdown)
					Engine::Get()->RemoveModule(info.module);

				OnUnregisteredNewModule.Invoke(info);

				return FreeModuleLibrary(platformModule);
			}

		return false;
	}

	std::wstring ModuleService::GetModuleDirectoryPath(const wchar_t* modulePath)
	{		
		fs::path path(modulePath);
		std::wstring fullPath = L"";

		if (path.has_parent_path() == false)
		{
			// engine module
			return ::Paths::GetEngineModulePaths() + WIDE("\\") + modulePath;
		}
		else if (path.is_absolute())
		{
			// full module path
			return modulePath;
		}
		else
		{
		}

		return fullPath;
	}

	std::wstring ModuleService::GetModuleLibraryPath(const wchar_t* path)
	{
		const std::string moduleName = fs::path(path).filename().string();

		return (fs::path(path) / "Cache\\Bin\\" / VARLET_CONFIGURATION_NAME / moduleName).wstring();
	}

	void ModuleService::LoadDependencies(ModuleInfo& moduleInfo)
	{
		const fs::path path(moduleInfo.modulePath);
		const std::string moduleName = path.filename().string();

		const fs::path dependeciesPath = path / (moduleName + ".vmodule");

		std::wifstream moduleFile(dependeciesPath);
 		assert(moduleFile.is_open());

		std::wstring name;

		while (std::getline(moduleFile, name))
		{
			const auto info = LoadModuleInternal(name.c_str(), true);
			const int32_t index = IndexWithName(info->modulePath);
			assert(index >= 0);

			moduleInfo.dependencyIndexes.push_back(index);
		}

		moduleFile.close();
	}

	int32_t ModuleService::IndexWithName(const std::wstring& path)
	{
		const auto it = std::find_if(_registered.begin(), _registered.end(), [&path](const ModuleInfo& info)
			{
				return info.modulePath == path;
			});

		return it != _registered.end()
			? std::distance(_registered.begin(), it)
			: -1;
	}
}
