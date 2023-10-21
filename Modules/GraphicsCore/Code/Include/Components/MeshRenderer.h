#pragma once

#include "GraphicsCoreCore.h"
#include "Renderer.h"
#include "MeshRenderer.generated.h"

class Mesh;

REFLECTABLE()
class GRAPHICSCORE_API MeshRenderer final : public Renderer
{
	GENERATION

private:

	FIELD(EditorAccessible)
	Mesh* _mesh = nullptr;

public:

	void SetMesh(Mesh* mesh);

	Mesh* GetMesh() const override;
};
