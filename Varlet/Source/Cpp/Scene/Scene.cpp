#include "Scene.h"
#include "Entity.h"
#include "Module.h"

Event<> Scene::SceneChangedEvent;
Scene* Scene::_current;

void Scene::Update()
{
	for (auto& entity : _current->_entities)
		entity->Update();
}

void Scene::Load(Scene* scene)
{
	// TODO make scena loading
	_current = scene;
	SceneChangedEvent.Invoke();
}

Entity* Scene::CreateEntity()
{
	const auto entity = new Entity();
	_current->_entities.push_back(entity);

	VARLET_LOG(LevelType::Normal, "Scene::CreateEntity()");

	return entity;
}

const std::vector<Entity*>& Scene::GetAll()
{
	return _current->_entities;
}

void Scene::Destroy(Entity* entity)
{
	std::remove(_current->_entities.begin(), _current->_entities.end(), entity);
	entity->OnDestroyed();
	delete entity;
}

void Scene::Shutdown()
{
	for (auto& entity : _current->_entities)
	{
		entity->OnDestroyed();
		delete entity;
	}
}
