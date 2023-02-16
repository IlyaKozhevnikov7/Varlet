#pragma once

#include "Common.h"
#include "Module.h"

namespace Varlet::Core
{
	class CORE_API Engine final
	{
		friend class ModuleService;

	private:

		static Engine _instance;

		std::vector<Module*> _modules;
		std::vector<IUpdatebleModule*> _updatebleModules;

		bool _isRunning;

	public:

		~Engine() = default;

		// new api 

		void Initialize(const char* projcetDirectory);

		// old api

		static Engine* Get();

		void InitModules();

		void Run();
			
		void Shutdown();

		static void WantExit();

	private:

		Engine();

		bool IsRunning() const;

		void AddModule(Module* module);

		void RemoveModule(Module* module);
	};
}
