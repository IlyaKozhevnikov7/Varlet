#pragma once

#include "GraphicsCoreCore.h"
#include "Components/Script.h"
#include "Types/Color.h"
#include "LightSource.generated.h"

REFLECTABLE(Abstract)
class GRAPHICSCORE_API LightSource : public Script
{
	GENERATION

public:

	static Event<LightSource*> OnLightSourceCreated;
	static Event<LightSource*> OnLightSourceDestroyed;

	FIELD(EditorAccessible)
	Color color = Color::White;

	FIELD(EditorAccessible)
	float intensity = 1.f;

public:

	void Start() override;

	void OnDestroyed() override;
};
