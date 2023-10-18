#include "Transform.h"

void Transform::Translate(const glm::vec3& delta)
{
    position += delta;
}

void Transform::Rotate(const glm::vec3& delta)
{    
    constexpr glm::vec3 maxRotation = glm::vec3(360.f);
    rotation = glm::mod(rotation + delta, maxRotation);
}

glm::vec3 Transform::GetForward() const
{
    return GetOrientation() * glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 Transform::GetRight() const
{
    return GetOrientation() * glm::vec3(1.f, 0.f, 0.f);
}

glm::vec3 Transform::GetUp() const
{
    return GetOrientation() * glm::vec3(0.f, 1.f, 0.f);
}

glm::quat Transform::GetOrientation() const
{
    return glm::quat(glm::radians(rotation));
}

glm::mat4 Transform::GetModelMatrix() const
{
    glm::mat4 model = glm::translate(glm::mat4(1.f), position);
    model *= glm::mat4_cast(GetOrientation());
    model = glm::scale(model, scale);

    return model;
}
