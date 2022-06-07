#pragma once

#include "Varlet.h"
#include "Context.h"

namespace Varlet
{
	class CORE_API Engine
	{
	private:
		
		ContextAPI* _context;

		std::vector<Module*> _modules;
		std::vector<IUpdatebleModule*> _updatebleModules;

	public:

		Engine();

		~Engine();

		void Init();

		void RegisterTargetModule(Module* targetModule);
	
		void InitModules();

		void Run();

		void Shutdown();
	};
}
