#include "Entity.h"
#include "Component.h"
#include "Script.h"

Event<Entity*, Component*> Entity::NewComponentCreatedEvent;

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

const std::vector<Component*>& Entity::GetComponents() const
{
	return _components;
}
