#pragma once

#include "VarletCore.h"
#include "Component.h"

namespace Varlet 
{
	class CameraCore;
}

class Transform;

class CORE_API Camera final : public Component
{
private:

	Varlet::CameraCore* _core;
	Transform* transform;

public:

	Camera();

	void Start() override;

	void Update() override;

};

