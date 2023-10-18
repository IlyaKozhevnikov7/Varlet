#pragma once

#include "Component.h"
#include "Transform.generated.h"

enum class Space : uint8_t
{
	World = 0,
	Local
};

REFLECTABLE()
class VARLET_API Transform final : public Component
{
	GENERATION

public:

	FIELD(EditorAccessible)
	glm::vec3 position = glm::vec3(1.f);

	FIELD(EditorAccessible)
	glm::vec3 rotation = glm::vec3(0.f);

	FIELD(EditorAccessible)
	glm::vec3 scale = glm::vec3(1.f);

public:

	// TODO translate in local space
	void Translate(const glm::vec3& delta);

	void Rotate(const glm::vec3& delta);

	glm::vec3 GetForward() const;

	glm::vec3 GetRight() const;

	glm::vec3 GetUp() const;

	glm::quat GetOrientation() const;

	glm::mat4 GetModelMatrix() const;
};

