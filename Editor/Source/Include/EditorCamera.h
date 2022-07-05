#pragma once

#include "VarletFramework.h"

class EditorCamera final : public Component
{
private:

	Camera* _camera;
	Transform* _transform;

public:

	void InternalStart();

	void InternalUpdate();

	const Varlet::Texture* GetRendereTexture() const;
};

