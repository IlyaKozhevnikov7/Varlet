#pragma once

#include "VarletCore.h"
#include "Component.h"

namespace Varlet 
{
	class Texture;
	class CameraCore;
}

class Transform;

class CORE_API Camera final : public Component
{
private:

	Varlet::CameraCore* _core;

public:

	Camera();

	void Update() override;

	const glm::mat4& GetView() const;

	const glm::mat4& GetProjection() const;

	const glm::mat4& GetViewProjection() const;
	
	const Varlet::CameraCore* GetCore() const;

	const Varlet::Texture* GetRendereTexture() const;

	void OnResize(const int32_t& width, const int32_t& height) const;
};

