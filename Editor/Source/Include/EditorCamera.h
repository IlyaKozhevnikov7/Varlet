#pragma once

#include "VarletFramework.h"

class EditorCamera final : public Component
{
private:

	Camera* _camera;
	Transform* _transform;

	float _sensitivity = 0.2f;
	glm::vec2 _lastMousePos;

	float _speed = 10.f;

public:

	void InternalStart();

	void InternalUpdate();

	void UpdateMoveAndRotation();

	const Varlet::Texture* GetRendereTexture() const;

	Transform* GetTransform() const;

private:

	float ConstrainPitch(const float& mouseDelta) const;
};

