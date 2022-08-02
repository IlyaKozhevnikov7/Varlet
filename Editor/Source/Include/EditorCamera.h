#pragma once

#include "VarletFramework.h"

class EditorCamera final : public Component
{
private:

	Camera* _camera;
	Camera* _selectedCamera;

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

	int8_t* ReadSelectedPixel(const int32_t& x, const int32_t& y) const;

	void OnResize(const int32_t& width, const int32_t& height) const;
};

