#pragma once

#include "VarletCore.h"
#include "Core/ComponentType.h"

class Object
{
public:
	
	Object() = default;
	virtual ~Object() = default;

	virtual Varlet::TypeInfo GetType();
};

