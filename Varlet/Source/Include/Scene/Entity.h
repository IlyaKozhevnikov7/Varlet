#pragma once

#include "VarletCore.h"

class Component;

namespace Varlet
{
	class Entity final
	{
	public:

		CORE_API static Event<Entity*, Component*> NewComponentCreatedEvent;

	private:

		std::vector<Component*> _components;

	public:

		void Update();

		template<class T>
		T* AddComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "Template must have component type");

			auto component = new T();
			auto componentBase = dynamic_cast<Component*>(component);

			_components.push_back(componentBase);
			componentBase->SetOwner(this);

			NewComponentCreatedEvent.Invoke(this, componentBase);

			return component;
		}

		template<class T>
		T* GetComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "Template must have component type");

			for (auto component : _components)
				if (auto correctComponent = dynamic_cast<T*>(component))
					return correctComponent;

			return nullptr;
		}

		template<class T>
		bool HasComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "Template must have component type");
		
			for (auto component : _components)
				if (auto correctComponent = dynamic_cast<T*>(component))
					return true;
		
			return false;
		}
	};
}
