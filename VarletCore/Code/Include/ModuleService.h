#pragma once

#include "Common.h"
#include "Platform/Current.h"

namespace Varlet::Core
{
	class Module;

	class CORE_API ModuleService final
	{
		struct ModuleInfo final
		{
			Platform::Module platformModule;
			Module* module;
		};

	private:

		static std::unordered_map<std::string, ModuleInfo> _registered;
			
	public:

		static Module* LoadModule(const char* path, const bool& withEngineRegistration = true);

		static bool FreeModule(const char* path);

		static void FreeModules();
	};
}