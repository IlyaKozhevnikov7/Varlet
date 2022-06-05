#pragma once

#include "Core/Module.h"
#include <iostream>

extern Varlet::Module* targetModule;

int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
		std::cout << argv[i] << std::endl;

	if (targetModule == nullptr)
		return 1;

	targetModule->Init();

	if (auto updatebleNodule = dynamic_cast<Varlet::IUpdatebleModule*>(targetModule))
	{
		for (int i = 0; i < 5; i++)
			updatebleNodule->Update();
	}

	targetModule->Shutdown();

	return 0;
}