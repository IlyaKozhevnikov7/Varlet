#pragma once

#include "VarletCore.h"
#include "Core/ComponentType.h"

namespace Varlet
{
	struct TypeInfo;
}

class Entity;

class CORE_API Component
{
private:

	Entity* _owner;

public:

	Component();

	virtual ~Component() = default;

	Entity* GetOwner();

	void SetOwner(Entity* owner);

	virtual void OnConstructed();

	virtual void Start();

	virtual void OnDestroyed();

	virtual Varlet::TypeInfo GetType();
};
