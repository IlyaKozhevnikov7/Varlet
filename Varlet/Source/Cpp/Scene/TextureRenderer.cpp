#include "TextureRenderer.h"
#include "Rendering/RendererAPI.h"
#include "Rendering/Texture.h"
#include "Rendering/Mesh.h"
#include "Material.h"

EMPTY_PROPERTY_BLOCK(TextureRenderer)

Mesh* TextureRenderer::_mesh = nullptr;

void TextureRenderer::OnConstructed()
{
	BaseClass::OnConstructed();

	if (_mesh == nullptr)
	{
		_mesh = new Mesh();
		_mesh->positions =
		{
			{ -1.f,  1.f, 0.f },
			{ -1.f, -1.f, 0.f },
			{  1.f, -1.f, 0.f },
			{  1.f,  1.f, 0.f }
		};
		
		_mesh->uvs =
		{
			{ 0.f, 1.f },
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f }
		};
		
		_mesh->indices = 
		{ 
			0, 1, 2,
			2, 3, 0 
		};
		
		_mesh->CommitChanges();
	}
}

const Varlet::Texture* TextureRenderer::GetTexture() const
{
	return _texture;
}

void TextureRenderer::SetTexture(Varlet::Texture* texture)
{
	if (_texture != texture)
	{
		_texture = texture;

		GenerateMesh(_texture->GetWidth(), _texture->GetHeight());

		for (const auto& material : materials)
			material->SetTexture("u_MainTexture", _texture);
	}
}

Mesh* TextureRenderer::GetMesh() const
{
	return _mesh;
}

void TextureRenderer::OnPreRender()
{
	_mesh->positions =
	{
		{ _verticesPosition[0],  _verticesPosition[1], 0.f },
		{ _verticesPosition[2],  _verticesPosition[3], 0.f },
		{ _verticesPosition[4],  _verticesPosition[5], 0.f },
		{ _verticesPosition[6],  _verticesPosition[7], 0.f }
	};

	_mesh->CommitChanges();
}

void TextureRenderer::GenerateMesh(const int32_t& width, const int32_t& height)
{
	const float aspect = static_cast<float>(width) / static_cast<float>(height);

	const float xHalf = aspect > 1.f
		? aspect
		: 1.f;

	const float yHalf = aspect < 1.f
		? static_cast<float>(height) / static_cast<float>(width)
		: 1.f;

	_verticesPosition[0] = -xHalf;
	_verticesPosition[1] = yHalf;

	_verticesPosition[2] = -xHalf;
	_verticesPosition[3] = -yHalf;

	_verticesPosition[4] = xHalf;
	_verticesPosition[5] = -yHalf;

	_verticesPosition[6] = xHalf;
	_verticesPosition[7] = yHalf;
}
