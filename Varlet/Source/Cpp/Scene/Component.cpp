#include "Component.h"
#include "ComponentType.h"

Component::Component()
{
	_owner = nullptr;
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

void Component::OnConstructed()
{
}

void Component::Start()
{
}

void Component::OnDestroyed()
{
}

Varlet::TypeInfo Component::GetType()
{
	Varlet::TypeInfo info("Component");
	return info;
}
