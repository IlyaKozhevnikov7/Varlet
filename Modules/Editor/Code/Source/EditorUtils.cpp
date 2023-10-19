#include "EditorUtils.h"
#include "Scene.h"
#include "Components/Transform.h"

Entity* EditorUtils::CreateDefautlEntity()
{
	const auto entity = Scene::CreateEntity();
	entity->AddComponent<Transform>();

	return entity;
}
