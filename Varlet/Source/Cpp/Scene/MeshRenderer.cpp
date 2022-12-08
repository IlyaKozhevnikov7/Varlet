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

Mesh* MeshRenderer::GetMesh() const
{
	return _mesh;
}

