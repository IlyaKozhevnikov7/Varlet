#pragma once

#include "Varlet.h"

namespace Varlet
{
	class VARLET_API Engine
	{
	private:

		std::vector<Module*> _modules;
		std::vector<IUpdatebleModule*> _updatebleModules;

	public:

		Engine() = default;

		~Engine() = default;

		void Init();

		void RegisterTargetModule(Module* targetModule);
	
		void Run();

		void Shutdown();
	};
}
