#pragma once

#include "VarletCore.h"

class Component;

namespace Varlet
{
	class Entity final
	{
	public:

		std::vector<Component*> _components;

	public:

		void Update();

		template<class T>
		T* AddComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "Template must have component type");

			T* component = new T();
			_components.push_back(dynamic_cast<Component*>(component));

			return component;
		}
	};
}
