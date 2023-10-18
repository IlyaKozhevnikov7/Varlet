#pragma once

#include "Common.h"
#include "Platform/Current.h"

namespace Varlet::Core
{
	class Module;

	struct ModuleDescriptor final
	{
		int32_t id;
	};

	struct ModuleInfo final
	{
		Platform::Module platformModule;
		Module* module;
		std::wstring libraryPath;

#ifdef VARLET_DEVELOPMENT
		std::wstring modulePath;
		std::vector<int32_t> dependencyIndexes;
#endif // VARLET_DEVELOPMENT
	};

	class CORE_API ModuleService final
	{
	public:

		static Event<const ModuleInfo&> OnRegisteredNewModule;
		static Event<const ModuleInfo&> OnUnregisteredNewModule;

	private:

		static std::vector<ModuleInfo> _registered;
			
	public:

		static Module* LoadModule(const wchar_t* modulePath, bool registeredByEngine = true);
		
		// TODO: make free module by varlet module
		static bool FreeModule(Platform::Module platformModule);

		static bool FreeModuleLibrary(Platform::Module platformModule);

		static void FreeModules();

		static Platform::Module GetPlatformModule(Module* module);

		static std::wstring GetModulePath(Module* module);
		
		static int32_t GetModuleIdByPath(const std::wstring_view path);

		static std::wstring GetModulePathById(int32_t id);

	private:

		static ModuleInfo* LoadModuleInternal(const wchar_t* modulePath, bool registeredByEngine);

		static std::pair<Platform::Module, Module*> LoadModuleFromLibrary(const wchar_t* path);

		static bool FreeModuleInternal(Platform::Module platformModule, bool callModuleShutdown);

		static std::wstring GetModuleDirectoryPath(const wchar_t* modulePath);

		static std::wstring GetModuleLibraryPath(const wchar_t* path);

		static void LoadDependencies(ModuleInfo& moduleInfo);
		
		static int32_t IndexWithName(const std::wstring& path);
	};
}