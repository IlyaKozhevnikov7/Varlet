#include "Script.h"

START_PROPERTY_BLOCK(Script)
PROPERTY("Activity", _isActive)
END_PROPERTY_BLOCK

void Script::OnConstructed()
{
	BaseClass::OnConstructed();

	_isActive = true;
}

bool Script::IsActive() const
{
	return _isActive;
}

void Script::SetActive(const bool& activeState)
{
	if (_isActive == activeState)
		return;

	_isActive = activeState;

	if (_isActive)
		OnActivated();
	else
		OnDeactivated();
}

void Script::Update()
{
}

void Script::OnActivated()
{
}

void Script::OnDeactivated()
{
}
