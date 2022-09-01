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

private:

	static int32_t _idCounter;

	int32_t _renderId;
	std::vector<Material*> _materials;

public:

	virtual void OnConstructed() override;

	const int32_t& GetRenderId() const;

	std::vector<Material*>& GetMaterials();

	virtual const std::vector<Varlet::VertexArray*>* GetVertices() const = 0;
};

