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

	bool _isControlled;

public:

	void InternalStart();

	void InternalUpdate();

	void UpdateMoveAndRotation();

	void SetControl(const bool&& control);

	bool IsControlled() const;

	const Varlet::Texture* GetRendereTexture() const;

	void OnResize(const int32_t& width, const int32_t& height) const;

	Transform* GetTransform() const;
};

