#pragma once

#include "VarletCore.h"
#include "Core/Reflection.h"
#include "Core/Object.h"

class Entity;

class CORE_API Component : public Object
{
	TYPE_GENERATION(Component, Object)

private:

	Entity* _owner;

public:

	Entity* GetOwner();

	void SetOwner(Entity* owner);

	virtual void OnConstructed();

	virtual void Start();

	virtual void OnDestroyed();
};