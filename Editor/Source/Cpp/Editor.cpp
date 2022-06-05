#include "Editor.h"
#include <iostream>

void Editor::Init()
{
	Module::Init();

	std::cout << "Editor init" << std::endl;
}

void Editor::Shutdown()
{
	std::cout << "Editor shutdown" << std::endl;
}

void Editor::Update()
{
	std::cout << "Editor update" << std::endl;
}
