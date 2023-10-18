#include "Script.h"

void Script::OnConstructed()
{
	Component::OnConstructed();

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
