#pragma once

#include "Core/Module.h"

#define MAKE_TARGET_SYSTEM(SystemType) MAKE_TARGET_SYSTEM_IMPLEMENT(SystemType)
#define MAKE_TARGET_SYSTEM_IMPLEMENT(SystemType) Varlet::Module* targetModule = new SystemType();