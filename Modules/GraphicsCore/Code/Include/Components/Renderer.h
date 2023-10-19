#pragma once

#include "GraphicsCoreCore.h"
#include "Components/Component.h"
#include "Renderer.generated.h"

class Mesh;
class Material;

REFLECTABLE(Abstract)
class GRAPHICSCORE_API Renderer : public Component
{
	GENERATION

public:

	static Event<Renderer*> OnRendererCreated;
	static Event<Renderer*> OnRendererDestroyed;

	Transform* _transfrom = nullptr;

	FIELD(EditorAccessible)
	bool isVisible = true;

	FIELD(EditorAccessible)
	Material* material = nullptr;

public:

	void Start() override;

	void OnDestroyed() override;

	virtual Mesh* GetMesh() const { return nullptr; }

	glm::mat4 GetModelMatrix();

	virtual void OnPreRender();
};
