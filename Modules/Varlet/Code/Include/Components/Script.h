#pragma once

#include "Component.h"
#include "Script.generated.h"

REFLECTABLE(Abstract)
class VARLET_API Script : public Component
{
	GENERATION

private:

	FIELD(EditorAccessible)
	bool _isActive;

public:
	
	~Script() override = default;

	void OnConstructed() override;

	bool IsActive() const;

	void SetActive(const bool& activeState);

	virtual void Update();

protected:

	virtual void OnActivated();

	virtual void OnDeactivated();
};
