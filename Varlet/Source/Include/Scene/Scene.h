#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class Entity;

	class Scene final
	{
	private:

		std::vector<Entity*> _entities;

	public:

		Scene() = default;
		~Scene() = default;

		void Update();

		CORE_API Entity* CreateEntity();
	};
}