#include "Component.h"
#include "ComponentType.h"

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

bool Component::IsActive() const
{
	return _isActive;
}

void Component::OnConstructed()
{
}

void Component::Start()
{
}

void Component::Update()
{
}

Varlet::TypeInfo Component::GetType()
{
	Varlet::TypeInfo info("Component");
	info.properties.push_back(Varlet::Property("Activity", _isActive));
	return info;
}

void Component::OnActivated()
{
}

void Component::OnDeactivated()
{
}