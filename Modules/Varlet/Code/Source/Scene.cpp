#include "Scene.h"
#include "Entity.h"
#include "Component.h"

Event<Scene*, Scene*> Scene::SceneChangedEvent;
Scene* Scene::_current = nullptr;

void Scene::Update()
{
	if (_current->_shouldUpdate == false)
		return;

	for (auto entity : _current->_entities)
		entity->Update();
}

void Scene::SetShouldUpdate(bool should)
{
	if (_current != nullptr)
		_current->_shouldUpdate = should;

	VT_LOG(Normal, __FUNCTION__);
}

Scene* Scene::Get()
{
	return _current;
}

void Scene::Load(Scene* scene)
{
	const auto lastScene = _current;
	_current = scene;

	SceneChangedEvent.Invoke(lastScene, _current);

	if (lastScene != nullptr)
	{
		lastScene->Shutdown();
		delete lastScene;
	}

	for (auto entity : _current->_entities)
		entity->Start();
}

Entity* Scene::CreateEntity()
{
	const auto entity = new Entity();
	_current->_entities.push_back(entity);

	VARLET_LOG(Normal, WIDE("Scene::CreateEntity()"));

	return entity;
}

const std::vector<Entity*>& Scene::GetAllEntities()
{
	return _current->_entities;
}

void Scene::Destroy(Entity* entity)
{
	const auto it = std::find(_current->_entities.begin(), _current->_entities.end(), entity);

	if (it != _current->_entities.end())
		_current->_entities.erase(it);

	entity->OnDestroyed();

	delete entity;
}

void Scene::Shutdown()
{
	for (auto entity : _entities)
	{
		entity->OnDestroyed();
		delete entity;
	}
}
