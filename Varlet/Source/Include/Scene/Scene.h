#pragma once

#include "VarletCore.h"
#include "VarletFramework.h"

namespace Varlet
{
	class GameModule;
}

class Entity;

class Scene final
{
public:

	CORE_API static Event<> SceneChangedEvent;

private:

	CORE_API static Scene* _current;

	std::vector<Entity*> _entities;

public:

	Scene() = default;

	~Scene() = default;

	static void Update();

	CORE_API static void Load(Scene* scene);

	CORE_API static Entity* CreateEntity();

	CORE_API static const std::vector<Entity*>& GetAll();

	CORE_API static void Destroy(Entity* entity);

	template<typename Predicate>
	static const Entity* Find(const Predicate&& predicate)
	{
		for (const auto entity : _current->_entities)
			if (predicate(entity))
				return entity;
		
		return nullptr;
	}

	static void Shutdown();
};