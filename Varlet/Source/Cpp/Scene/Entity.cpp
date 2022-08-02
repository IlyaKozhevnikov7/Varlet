#include "Entity.h"
#include "Component.h"

Event<Entity*, Component*> Entity::NewComponentCreatedEvent;

void Entity::Update()
{
	for (auto component : _components)
		if (component->IsActive())
			component->Update();
}

void Entity::OnDestroyed()
{
	for (auto component : _components)
	{
		component->SetActive(false);
		component->OnDestroyed();
		delete component;
	}
}

const std::vector<Component*>& Entity::GetComponents() const
{
	return _components;
}
