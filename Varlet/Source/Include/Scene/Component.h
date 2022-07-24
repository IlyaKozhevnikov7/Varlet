#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class Entity;
	struct TypeInfo;
}

class CORE_API Component
{
protected:

	Varlet::Entity* _owner;

private:

	bool _isActive;

public:

	// todo make private
	void SetOwner(Varlet::Entity* owner);

	void SetActive(bool activeState);

	bool IsActive() const;

	virtual void OnConstructed();

	virtual void Start();

	virtual void Update();

	virtual Varlet::TypeInfo GetType();

protected:

	virtual void OnActivated();

	virtual void OnDeactivated();
};
