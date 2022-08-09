#pragma once

#include "Component.h"

class CORE_API Script : public Component
{
	TYPE_GENERATION(Script, Component)

private:

	bool _isActive;

public:

	void OnConstructed() override;

	bool IsActive() const;

	void SetActive(const bool& activeState);

	virtual void Update();

protected:

	virtual void OnActivated();

	virtual void OnDeactivated();
};

