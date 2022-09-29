#include "Entity.h"
#include "Component.h"
#include "Script.h"

Event<Entity*, Component*> Entity::NewComponentCreatedEvent;

Entity::Entity()
{
	_id = reinterpret_cast<size_t>(this);
}

void Entity::Update()
{
	for (auto& script : _scripts)
		if (script->IsActive())
			script->Update();
}

void Entity::OnDestroyed()
{
	for (auto& script : _scripts)
		script->SetActive(false);

	for (auto& component : _components)
	{
		component->OnDestroyed();
		delete component;
	}
}

uint32_t Entity::GetId() const
{
	return _id;
}

const std::vector<Component*>& Entity::GetComponents() const
{
	return _components;
}
