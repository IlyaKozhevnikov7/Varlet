#pragma once

#include "InputCore.h"
#include "Module.h"

class INPUT_API InputModule final : public Varlet::Core::Module 
{
public:

	bool Initialize() override;
};