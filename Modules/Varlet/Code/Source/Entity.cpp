#include "Entity.h"
#include "Component.h"
#include "Script.h"

Event<Entity*, Component*> Entity::NewComponentCreatedEvent;

void Entity::Start()
{
	for (auto component : _components)
	{
		component->Start();

		if (auto script = dynamic_cast<Script*>(component))
			_scripts.push_back(script);
	}
}

void Entity::Update()
{
	for (auto script : _scripts)
		if (script->IsActive())
			script->Update();
}

void Entity::OnDestroyed()
{
	for (auto script : _scripts)
		script->SetActive(false);

	for (auto component : _components)
	{
		component->OnDestroyed();
		delete component;
	}
}

const std::vector<Component*>& Entity::GetComponents() const
{
	return _components;
}

Component* Entity::AddComponent(const Varlet::Core::Type* type)
{
	for (const auto component : _components)
		if (type->CheckIsA(component->GetType()))
			return nullptr;

	Component* component = CastMemory<Component>(type, (int8_t*)type->CreateInstance());

	RegisterComponent(component);

	return component;
}

void Entity::RemoveComponent(Component* component)
{
	_components.erase(std::remove(_components.begin(), _components.end(), component), _components.end());
	_scripts.erase(std::remove(_scripts.begin(), _scripts.end(), component), _scripts.end());

	component->OnDestroyed();

	delete component;
}

bool Entity::ShouldDestroy(const Entity* entity)
{
	return entity->_shouldDestroy;
}

void Entity::RegisterComponent(Component* component)
{
	_components.push_back(component);

	if (const auto script = Cast<Script>(component))
		_scripts.push_back(script);
	
	component->OnConstructed();
	component->SetOwner(this);
	component->Start();

	NewComponentCreatedEvent.Invoke(this, component);
}
