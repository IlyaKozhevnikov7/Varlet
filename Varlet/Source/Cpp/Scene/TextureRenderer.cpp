#include "TextureRenderer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Material.h"

EMPTY_PROPERTY_BLOCK(TextureRenderer);

std::vector<Varlet::VertexArray*> TextureRenderer::_vertices;

void TextureRenderer::OnConstructed()
{
	BaseClass::OnConstructed();

	if (_vertices.size() == 0)
	{
		_vertices.push_back(Varlet::RendererAPI::CreateVertexArray(
		{
			{
				{{ -0.5f,  0.5f, 0.f }, { 0.f, 0.f, -1.f }, { 0.f, 1.f }},
				{{ -0.5f, -0.5f, 0.f }, { 0.f, 0.f, -1.f }, { 0.f, 0.f }},
				{{  0.5f, -0.5f, 0.f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f }},
				{{  0.5f,  0.5f, 0.f }, { 0.f, 0.f, -1.f }, { 1.f, 1.f }}
			},
			{
				0, 1, 2,
				2, 3, 0
			}
		}));
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

		int32_t width, height;
		_texture->GetResolution(&width, &height);
		GenerateMesh(width, height);

		for (const auto& material : materials)
			material->SetSampler2D("u_MainTexture", _texture);
	}
}

const std::vector<Varlet::VertexArray*>* TextureRenderer::GetVertices() const
{
	return &_vertices;
}

void TextureRenderer::OnPreRender()
{
	_vertices[0]->Bind();
	_vertices[0]->SetData(0, _vertices[0]->vertices.size() * sizeof(glm::vec3), &_verticesPosition[0]);
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
	_verticesPosition[2] = 0.f;

	_verticesPosition[3] = -xHalf;
	_verticesPosition[4] = -yHalf;
	_verticesPosition[5] = 0.f;

	_verticesPosition[6] = xHalf;
	_verticesPosition[7] = -yHalf;
	_verticesPosition[8] = 0.f;

	_verticesPosition[9] = xHalf;
	_verticesPosition[10] = yHalf;
	_verticesPosition[11] = 0.f;
}
