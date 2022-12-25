#pragma once

#include "VarletCore.h"
#include "Module.h"

namespace Varlet
{
	class CORE_API Engine final
	{
	private:

		std::vector<Module*> _modules;
		std::vector<IUpdatebleModule*> _updatebleModules;

		bool _isRunning;

	public:

		~Engine() = default;

		static Engine* Get();

		void RegisterTargetModule(Module* targetModule);

		void InitModules();

		void Run();

		void Shutdown();

		static void WantExit();

	private:

		Engine();

		bool IsRunning() const;
	};
}
