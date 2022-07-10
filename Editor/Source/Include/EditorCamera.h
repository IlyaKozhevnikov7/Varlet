#pragma once

#include "VarletFramework.h"

class EditorCamera final : public Component
{
private:

	Camera* _camera;
	Transform* _transform;

	float sensitivity = 0.2f;

public:

	void InternalStart();

	void InternalUpdate();

	const Varlet::Texture* GetRendereTexture() const;

	Transform* GetTransform() const;

private:

	float ConstrainPitch(const float& mouseDelta) const;
};

