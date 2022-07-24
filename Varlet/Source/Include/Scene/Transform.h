#pragma once

#include "VarletCore.h"
#include "VarletFramework.h"

enum class Space : uint8_t
{
	World = 0,
	Local
};

class CORE_API Transform final : public Component
{
	friend class Camera;

	TYPE_GENERATION(Transform, Component)

private:

	glm::vec3 _position;
	glm::quat _rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
	glm::vec3 _scale = glm::vec3(1.f);

public:

	glm::vec3 GetPosition() const;

	glm::quat GetRotation() const;

	glm::vec3 GetScale() const;

	// TODO translate in local space
	void Translate(const glm::vec3& delta);

	void Rotate(const float& angle, glm::vec3 axis, const Space& relativeTo = Space::Local);

	glm::vec3 GetForward() const;

	glm::vec3 GetRight() const;

	glm::vec3 GetUp() const;

	glm::vec3 GetEulerAngles() const;
};

