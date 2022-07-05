#pragma once

#include "VarletCore.h"
#include "Component.h"

class Mesh;

class CORE_API MeshRenderer final : public Component
{
private:

	Mesh* _mesh;
	std::string modelPath;

public:

	void OnConstructed() override;

	void SetMesh(Mesh* mesh);

	const Mesh* GetMesh() const;
};
