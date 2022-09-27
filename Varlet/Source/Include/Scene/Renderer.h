#pragma once

#include "Component.h"

namespace Varlet
{
	class VertexArray;
}

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

	virtual const std::vector<Varlet::VertexArray*>* GetVertices() const = 0;

	glm::mat4 GetModelMatrix();

	virtual void OnPreRender();
};

