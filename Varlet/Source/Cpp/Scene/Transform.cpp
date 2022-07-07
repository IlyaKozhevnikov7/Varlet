#include "Transform.h"

glm::vec3 Transform::GetPosition() const
{
    return _position;
}

glm::vec3 Transform::GetRotation() const
{
    return _rotation;
}

glm::vec3 Transform::GetScale() const
{
    return _scale;
}

void Transform::Translate(const glm::vec3& delta)
{
    _position += delta;
}

void Transform::Rotate(const glm::vec3& delta)
{
    _rotation += delta;
}
