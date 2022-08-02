#pragma once

#include "VarletCore.h"

namespace Varlet
{
	struct TypeInfo;
}

class Entity;

class CORE_API Component
{
private:

	Entity* _owner;
	bool _isActive;

public:

	Component();

	virtual ~Component() = default;

	Entity* GetOwner();

	void SetOwner(Entity* owner);

	void SetActive(const bool& activeState);

	bool IsActive() const;

	virtual void OnConstructed();

	virtual void Start();

	virtual void Update();

	virtual void OnDestroyed();

	virtual Varlet::TypeInfo GetType();

protected:

	virtual void OnActivated();

	virtual void OnDeactivated();
};
