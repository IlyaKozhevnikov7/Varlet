#pragma once

#include "VarletCore.h"
#include "Component.h"

class Transform final : public Component
{
private:

	glm::vec3 _position;
	glm::vec3 _rotation; // TODO remake to quat
	glm::vec3 _scale;

public:

	glm::vec3 GetPosition() const;

	glm::vec3 GetRotation() const;

	glm::vec3 GetScale() const;
};

