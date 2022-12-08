#pragma once

#include "Renderer.h"

class Mesh;

namespace Varlet
{
	class Texture;
	class VertexArray;
}

class CORE_API TextureRenderer final : public Renderer
{
	TYPE_GENERATION(TextureRenderer, Renderer)

private:

	static Mesh* _mesh;

	Varlet::Texture* _texture;
	float _verticesPosition[4 * 2];

public:

	void OnConstructed() override;

	const Varlet::Texture* GetTexture() const;

	void SetTexture(Varlet::Texture* texture);

	Mesh* GetMesh() const override;

	void OnPreRender() override;

private:

	void GenerateMesh(const int32_t& width, const int32_t& height);
};

