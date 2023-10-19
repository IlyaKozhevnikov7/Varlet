#pragma once

#include "GraphicsCoreCore.h"
#include "LightSource.h"
#include "Types/Color.h"
#include "PointLight.generated.h"

REFLECTABLE()
class GRAPHICSCORE_API PointLight final : public LightSource
{
	GENERATION

public:

	FIELD(EditorAccessible)
	float attenuation = 0.064f;
};
