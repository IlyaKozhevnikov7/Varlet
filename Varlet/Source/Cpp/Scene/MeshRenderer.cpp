#include "MeshRenderer.h"
#include "Mesh.h"

int32_t MeshRenderer::_idCounter = 1;

const int32_t& MeshRenderer::GetRenderId() const
{
	return _renderId;
}

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

