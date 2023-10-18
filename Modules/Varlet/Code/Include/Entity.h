#pragma once

#include "VarletCore.h"
#include "Entity.generated.h"

class Component;
class Script;

REFLECTABLE()
class VARLET_API Entity final
{
	GENERATION

public:

	static Event<Entity*, Component*> NewComponentCreatedEvent;

private:

	bool _shouldDestroy;

	FIELD()
	std::vector<Component*> _components;

	std::vector<Script*> _scripts;

public:

	Entity() = default;

	void Start();

	void Update();

	void OnDestroyed();

	const std::vector<Component*>& GetComponents() const;

	Component* AddComponent(const Varlet::Core::Type* type);

	void RemoveComponent(Component* component);

	template<class T>
	T* AddComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "Template must have component type");

		auto component = new T();
		auto componentBase = dynamic_cast<Component*>(component);

		RegisterComponent(componentBase);

		return component;
	}

	template<class T>
	T* GetComponent() const
	{
		static_assert(std::is_base_of_v<Component, T>, "Template must have component type");

		for (auto component : _components)
			if (auto correctComponent = dynamic_cast<T*>(component))
				return correctComponent;

		return nullptr;
	}

	template<class T>
	bool HasComponent() const
	{
		static_assert(std::is_base_of_v<Component, T>, "Template must have component type");

		for (auto component : _components)
			if (auto correctComponent = dynamic_cast<T*>(component))
				return true;

		return false;
	}

	static bool ShouldDestroy(const Entity* entity);

private:

	void RegisterComponent(Component* component);
};
