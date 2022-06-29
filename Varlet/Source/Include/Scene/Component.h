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

private:

	bool _isActive;

public:

	void SetOwner(Varlet::Entity* owner);

	void SetActive(bool activeState);

	bool IsActive() const;

	virtual void Start();

	virtual void Update();

protected:

	virtual void OnActivated();

	virtual void OnDeactivated();
};
