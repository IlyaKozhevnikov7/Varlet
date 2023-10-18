#pragma once

#include "VarletCore.h"
#include "Scene.generated.h"

class Entity;

namespace Varlet
{
	class SceneUtils;
}

REFLECTABLE()
class Scene final
{
	GENERATION

	friend class Varlet::SceneUtils;

public:

	VARLET_API static Event<Scene*, Scene*> SceneChangedEvent;

private:

	VARLET_API static Scene* _current;

	FIELD()
	std::vector<Entity*> _entities;
	
	std::vector<Entity*> _entitiesToAdd;

	bool _shouldUpdate = true;

public:

	Scene() = default;

	~Scene() = default;

	static void Update();

	VARLET_API static void SetShouldUpdate(bool should);

	VARLET_API static Scene* Get();

	VARLET_API static void Load(Scene* scene);

	VARLET_API static Entity* CreateEntity();

	VARLET_API static const std::vector<Entity*>& GetAllEntities();

	VARLET_API static void Destroy(Entity* entity);

	template<typename Predicate>
	static Entity* Find(const Predicate&& predicate)
	{
		for (const auto entity : _current->_entities)
			if (predicate(entity))
				return entity;
		
		return nullptr;
	}

	template<typename Predicate>
	static std::vector<Entity*> FindAll(const Predicate&& predicate)
	{
		std::vector<Entity*> entities;

		for (const auto entity : _current->_entities)
			if (predicate(entity))
				entities.push_back(entity);

		return entities;
	}

	void Shutdown();
};
