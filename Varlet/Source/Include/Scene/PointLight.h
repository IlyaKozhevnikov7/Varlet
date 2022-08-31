#pragma once

#include "LightSource.h"

class CORE_API PointLight final : public LightSource
{
	TYPE_GENERATION(PointLight, LightSource);

public:

	glm::vec3 color;
	float linear;
	float quadratic;

public:

	void OnConstructed() override;
};

