#pragma once

#include "VarletCore.h"
#include "Component.h"

class Mesh;
class Material;

class CORE_API MeshRenderer final : public Component
{
private:

	static int32_t _idCounter;

	int32_t _renderId = _idCounter++;

	Mesh* _mesh;
	std::vector<Material*> _materials;
	std::string modelPath;

public:

	const int32_t& GetRenderId() const;

	void OnConstructed() override;

	void SetMesh(Mesh* mesh);

	const Mesh* GetMesh() const;

	std::vector<Material*>& GetMaterials();
};
