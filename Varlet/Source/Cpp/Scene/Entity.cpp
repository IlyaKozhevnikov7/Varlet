#include "Entity.h"
#include "Component.h"

namespace Varlet
{
    Event<Entity*, Component*> Entity::NewComponentCreatedEvent;

    void Entity::Update()
    {
        for (auto component : _components)
            component->Update();
    }
}
