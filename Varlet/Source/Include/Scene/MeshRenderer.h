#pragma once

#include "VarletCore.h"
#include "Renderer.h"

class Mesh;
class Material;

class CORE_API MeshRenderer final : public Renderer
{
	TYPE_GENERATION(MeshRenderer, Renderer)

private:

	Mesh* _mesh;
	std::string modelPath;

public:

	void OnConstructed() override;

	void SetMesh(Mesh* mesh);

	const std::vector<Varlet::VertexArray*>* GetVertices() const override;

};
