#pragma once

#include "Varlet.h"

namespace Varlet
{
	class CORE_API Engine
	{
	private:

		std::vector<Module*> _modules;
		std::vector<IUpdatebleModule*> _updatebleModules;

	public:

		Engine() = default;

		~Engine() = default;

		void Init();

		void RegisterTargetModule(Module* targetModule);
	
		void InitModules();

		void Run();

		void Shutdown();
	};
}
