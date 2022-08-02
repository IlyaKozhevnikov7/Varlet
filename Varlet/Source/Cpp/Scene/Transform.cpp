#include "Transform.h"
#include <GLM/gtx/quaternion.hpp>

START_PROPERTY_BLOCK(Transform)
PROPERTY("Position", position)
PROPERTY("Scale", scale)
END_PROPERTY_BLOCK

void Transform::Translate(const glm::vec3& delta)
{
    position += delta;
}

void Transform::Rotate(const float& angle, glm::vec3 axis, const Space& relativeTo)
{
    if (relativeTo == Space::World)
        axis = axis * rotation;

    rotation *= glm::angleAxis(glm::radians(angle), axis);
    rotation = glm::normalize(rotation);
}

glm::vec3 Transform::GetForward() const
{
    return rotation * glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 Transform::GetRight() const
{
    return rotation * glm::vec3(1.f, 0.f, 0.f);
}

glm::vec3 Transform::GetUp() const
{
    return rotation * glm::vec3(0.f, 1.f, 0.f);
}

glm::vec3 Transform::GetEulerAngles() const
{
    return glm::eulerAngles(rotation);
}
