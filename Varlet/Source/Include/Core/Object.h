#pragma once

#include "VarletCore.h"

namespace Varlet
{
	struct TypeInfo;
}

class CORE_API Object
{
public:
	
	Object() = default;
	virtual ~Object() = default;

	virtual Varlet::TypeInfo GetType();
};
