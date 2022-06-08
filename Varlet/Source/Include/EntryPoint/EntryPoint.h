#pragma once

#include "VarletCore.h"
#include "Core/Engine.h"
#include <iostream>

extern Varlet::Module* targetModule;

int32_t main(int32_t argc, char* argv[])
{
	if (targetModule == nullptr)
		return 1;

	for (int32_t i = 0; i < argc; i++)
		std::cout << argv[i] << std::endl;

	Varlet::Engine engine;
	
	engine.Init();

	engine.RegisterTargetModule(targetModule);
	engine.InitModules();

	engine.Run();

	engine.Shutdown();

	return 0;
}