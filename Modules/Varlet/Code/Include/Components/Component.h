#pragma once

#include "VarletCore.h"
#include "Entity.h"
#include "Component.generated.h"

REFLECTABLE(Abstract)
class VARLET_API Component
{
	GENERATION

private:

	FIELD()
	Entity* _owner = nullptr;

public:

	virtual ~Component() = default;

protected:

	Component() = default;

public:

	Entity* GetOwner();

	void SetOwner(Entity* owner);

	virtual void OnConstructed();

	virtual void Start();

	virtual void OnDestroyed();
};
