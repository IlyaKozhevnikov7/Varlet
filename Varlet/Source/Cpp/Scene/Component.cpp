#include "Component.h"

void Component::SetOwner(Varlet::Entity* owner)
{
	_owner = owner;
}

void Component::SetActive(bool activeState)
{
	_isActive = activeState;

	if (_isActive)
		OnActivated();
	else
		OnDeactivated();
}

void Component::Start()
{
}

void Component::Update()
{
}

void Component::OnActivated()
{
}

void Component::OnDeactivated()
{
}