#include "Component.h"

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
