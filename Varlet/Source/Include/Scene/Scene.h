#pragma once

#include "VarletCore.h"
#include "VarletFramework.h"

namespace Varlet
{
	class Entity;
	class GameModule;
}

class Scene final
{
public:

	CORE_API static Event<> SceneChangedEvent;

private:

	CORE_API static Scene* _current;

	std::vector<Varlet::Entity*> _entities;

public:

	Scene() = default;

	~Scene() = default;

	static void Update();

	CORE_API static void Load(Scene* scene);

	CORE_API static Varlet::Entity* CreateEntity();

	CORE_API static const std::vector<Varlet::Entity*>& GetAll();

	template<typename Predicate>
	static const Varlet::Entity* Find(const Predicate& predicate)
	{
		for (const auto entity : _current->_entities)
			if (predicate(entity))
				return entity;
		
		return nullptr;
	}
};