#pragma once

#include "Core/Module.h"
#include <iostream>
#include "Core/Engine.h"

extern Varlet::Module* targetModule;

int main(int argc, char* argv[])
{
	if (targetModule == nullptr)
		return 1;

	for (int i = 0; i < argc; i++)
		std::cout << argv[i] << std::endl;

	Varlet::Engine engine;
	
	engine.Init();
	engine.RegisterTargetModule(targetModule);

	engine.Run();

	engine.Shutdown();

	return 0;
}