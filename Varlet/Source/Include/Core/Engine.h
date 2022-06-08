#pragma once

#include "VarletCore.h"
#include "Context.h"
#include "Module.h"

namespace Varlet
{
	class CORE_API Engine final
	{
	private:
		
		ContextAPI* _context;

		std::vector<Module*> _modules;
		std::vector<IUpdatebleModule*> _updatebleModules;

		bool _isRunning;

	public:

		Engine();

		~Engine();

		void Init();

		void RegisterTargetModule(Module* targetModule);
	
		void InitModules();

		void Run();

		void Shutdown();

	private:

		bool IsRunning() const;
	};
}
