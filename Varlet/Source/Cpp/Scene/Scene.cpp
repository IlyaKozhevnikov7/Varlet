#include "Scene.h"
#include "Entity.h"

namespace Varlet
{
	void Scene::Update()
	{
		for (auto entity : _entities)
			entity->Update();
	}

	Entity* Scene::CreateEntity()
	{
		auto entity = new Entity();
		_entities.push_back(entity);

		VARLET_LOG(LevelType::Normal, "Scene::CreateEntity()");

		return entity;
	}
}
