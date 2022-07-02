#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

struct Vertex
{
public:

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinate;
};

struct VertexArrayData
{
public:

	std::vector<Vertex> vertices;
	std::vector<int32_t> indices;
};

namespace Varlet
{
	class CORE_API OpenGLVertexArray : public VertexArray
	{
	private:
		
		uint32_t _vao;
		uint32_t _vbo;
		uint32_t _ebo;

		std::vector<Vertex> _vertices;
		std::vector<int32_t> _indices;

	public:

		OpenGLVertexArray(const VertexArrayData& data);

		~OpenGLVertexArray() override;

		const uint32_t& GetVAO() const;

		bool IsIndexed() const;

		const int32_t GetElementsCount() const;
	};
}
