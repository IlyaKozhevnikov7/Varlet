#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class Entity;
}

class CORE_API Component
{
protected:

	Varlet::Entity* _owner;

public:

	void SetOwner(Varlet::Entity* owner);

	virtual void Start();

	virtual void Update();
};
