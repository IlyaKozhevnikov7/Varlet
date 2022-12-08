#pragma once

#include "Component.h"

namespace Varlet
{
	class VertexArray;
}

class Mesh;
class Material;

class CORE_API Renderer : public Component
{
	TYPE_GENERATION(Renderer, Component)

public:

	bool isVisible;
	std::vector<Material*> materials;

private:

	static int32_t _idCounter;

	int32_t _renderId;

public:

	virtual void OnConstructed() override;

	const int32_t& GetRenderId() const;

	virtual Mesh* GetMesh() const = 0;

	glm::mat4 GetModelMatrix();

	virtual void OnPreRender();
};

