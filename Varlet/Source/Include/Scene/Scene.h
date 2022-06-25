#pragma once

#include "VarletCore.h"
#include "Entity.h"

namespace Varlet
{
	class Scene final
	{
	private:

		std::vector<Entity*> _entities;

	public:

		Scene() = default;
		~Scene() = default;

		void Update();

		Entity* CreateEntity();
	};
}