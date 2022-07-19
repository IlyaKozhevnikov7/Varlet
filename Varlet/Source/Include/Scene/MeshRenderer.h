#pragma once

#include "VarletCore.h"
#include "Component.h"

class Mesh;
class Material;

class CORE_API MeshRenderer final : public Component
{
private:

	Mesh* _mesh;
	std::vector<Material*> _materials;
	std::string modelPath;

public:

	void OnConstructed() override;

	void SetMesh(Mesh* mesh);

	const Mesh* GetMesh() const;

	std::vector<Material*>& GetMaterials();
};
