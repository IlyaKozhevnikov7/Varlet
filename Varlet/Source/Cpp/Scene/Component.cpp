#include "Component.h"
#include "Reflection.h"

EMPTY_PROPERTY_BLOCK(Component);

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
	_owner = nullptr;
}

void Component::Start()
{
}

void Component::OnDestroyed()
{
}
