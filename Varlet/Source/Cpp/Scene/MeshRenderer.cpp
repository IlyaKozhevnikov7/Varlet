#include "MeshRenderer.h"
#include "Mesh.h"

EMPTY_PROPERTY_BLOCK(MeshRenderer);

void MeshRenderer::OnConstructed()
{
	BaseClass::OnConstructed();

	_mesh = Mesh::LoadModel(modelPath);
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
	if (_mesh != nullptr)
		delete _mesh;

	_mesh = mesh;
}

const std::vector<Varlet::VertexArray*>* MeshRenderer::GetVertices() const
{
	return _mesh != nullptr
		? &_mesh->GetSubMeshes()
		: nullptr;
}

