#include "Scene.h"
#include "Entity.h"
#include "Module.h"

Event<> Scene::SceneChangedEvent;
Scene* Scene::_current;

void Scene::Update()
{
	for (auto entity : _current->_entities)
		entity->Update();
}

void Scene::Load(Scene* scene)
{
	// TODO make scena loading
	_current = scene;
	SceneChangedEvent.Invoke();
}

Varlet::Entity* Scene::CreateEntity()
{
	const auto entity = new Varlet::Entity();
	_current->_entities.push_back(entity);

	VARLET_LOG(LevelType::Normal, "Scene::CreateEntity()");

	return entity;
}

const std::vector<Varlet::Entity*> Scene::FindAll()
{
	return _current->_entities;
}
