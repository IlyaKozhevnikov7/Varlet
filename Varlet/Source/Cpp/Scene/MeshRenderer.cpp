#include "MeshRenderer.h"
#include "Mesh.h"

void MeshRenderer::OnConstructed()
{
	_mesh = Mesh::LoadModel(modelPath);

}
void MeshRenderer::SetMesh(Mesh* mesh)
{
	if (_mesh != nullptr)
		delete _mesh;

	_mesh = mesh;
}

const Mesh* MeshRenderer::GetMesh() const
{
	return _mesh;
}

std::vector<Material*>& MeshRenderer::GetMaterials()
{
	return _materials;
}

