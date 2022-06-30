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

	void Start() override;

	void Update() override;

	const glm::mat4& GetView() const;

	const glm::mat4& GetProjection() const;

	const glm::mat4 GetViewProjection() const;
};

