#include "Entity.h"
#include "Component.h"

namespace Varlet
{
    void Entity::Update()
    {
        for (auto component : _components)
            component->Update();
    }
}
