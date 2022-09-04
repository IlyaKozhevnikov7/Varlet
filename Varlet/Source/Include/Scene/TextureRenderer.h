#pragma once

#include "Renderer.h"

namespace Varlet
{
	class Texture;
	class VertexArray;
}

class CORE_API TextureRenderer final : public Renderer
{
	TYPE_GENERATION(TextureRenderer, Renderer)

private:

	static std::vector<Varlet::VertexArray*> _vertices;

	Varlet::Texture* _texture;
	float _verticesPosition[4 * 3];

public:

	void OnConstructed() override;

	const Varlet::Texture* GetTexture() const;

	void SetTexture(Varlet::Texture* texture);

	const std::vector<Varlet::VertexArray*>* GetVertices() const override;

	void OnPreRender() override;

private:

	void GenerateMesh(const int32_t& width, const int32_t& height);
};

