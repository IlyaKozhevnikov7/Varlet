#include "Transform.h"
#include <GLM/gtx/quaternion.hpp>

START_PROPERTY_BLOCK(Transform)
PROPERTY("Position", _position)
PROPERTY("Scale", _scale)
END_PROPERTY_BLOCK

glm::vec3 Transform::GetPosition() const
{
    return _position;
}

glm::quat Transform::GetRotation() const
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

void Transform::Rotate(const float& angle, glm::vec3 axis, const Space& relativeTo)
{
    if (relativeTo == Space::World)
        axis = axis * _rotation;

    _rotation *= glm::angleAxis(glm::radians(angle), axis);
    _rotation = glm::normalize(_rotation);
}

glm::vec3 Transform::GetForward() const
{
    return _rotation * glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 Transform::GetRight() const
{
    return _rotation * glm::vec3(1.f, 0.f, 0.f);
}

glm::vec3 Transform::GetUp() const
{
    return _rotation * glm::vec3(0.f, 1.f, 0.f);
}

glm::vec3 Transform::GetEulerAngles() const
{
    return glm::eulerAngles(_rotation);
}
