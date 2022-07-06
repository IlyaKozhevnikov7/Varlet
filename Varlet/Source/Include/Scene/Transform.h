#pragma once

#include "VarletCore.h"
#include "VarletFramework.h"

class CORE_API Transform final : public Component
{
	friend class Camera;

private:

	glm::vec3 _position;
	glm::vec3 _rotation; // TODO remake to quat
	glm::vec3 _scale = glm::vec3(1.f);

public:

	glm::vec3 GetPosition() const;

	glm::vec3 GetRotation() const;

	glm::vec3 GetScale() const;

	// TODO translate in local space
	void Translate(const glm::vec3& delta);

	void Rotate(const glm::vec3& delta);
};

