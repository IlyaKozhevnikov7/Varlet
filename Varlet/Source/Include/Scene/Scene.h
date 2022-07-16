#pragma once

#include "VarletCore.h"

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

	static Scene* _current;

	std::vector<Varlet::Entity*> _entities;

public:

	Scene() = default;

	~Scene() = default;

	static void Update();

	CORE_API static void Load(Scene* scene);

	CORE_API static Varlet::Entity* CreateEntity();

	CORE_API static const std::vector<Varlet::Entity*> FindAll();
};