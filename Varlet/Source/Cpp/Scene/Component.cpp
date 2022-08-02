#include "Component.h"
#include "ComponentType.h"

Component::Component()
{
	_owner = nullptr;
	_isActive = true;
}

Entity* Component::GetOwner()
{
	return _owner;
}

void Component::SetOwner(Entity* owner)
{
	assert(_owner == nullptr && "Owner already initialized");
	_owner = owner;
}

void Component::SetActive(const bool& activeState)
{
	if (_isActive == activeState)
		return;

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

void Component::OnDestroyed()
{
}

void Component::OnDeactivated()
{
}