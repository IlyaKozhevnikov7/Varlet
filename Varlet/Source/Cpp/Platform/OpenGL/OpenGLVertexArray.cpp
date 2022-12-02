#include "OpenGL/OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Varlet
{
	OpenGLVertexArray::OpenGLVertexArray(const VertexArrayData& data)
	{
		vertices = data.vertices;
		indices = data.indices;

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> textureCoordinates;
		std::vector<glm::vec3> tangents;

		for (const auto& value : vertices)
		{
			positions.push_back(value.position);
			normals.push_back(value.normal);
			textureCoordinates.push_back(value.textureCoordinate);
			tangents.push_back(value.tangent);
		}

		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);

		glBindVertexArray(_vao);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0,																vertices.size() * sizeof(glm::vec3), &positions[0]);
		glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),							vertices.size() * sizeof(glm::vec3), &normals[0]);
		glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) * 2,						vertices.size() * sizeof(glm::vec2), &textureCoordinates[0]);
		glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * (sizeof(glm::vec3) * 2 + sizeof(glm::vec2)), vertices.size() * sizeof(glm::vec3), &tangents[0]);

		if (indices.size() > 0)
		{
			glGenBuffers(1, &_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
		}

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(vertices.size() * sizeof(glm::vec3)));
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(vertices.size() * sizeof(glm::vec3) * 2));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(vertices.size() * (sizeof(glm::vec3) * 2 + sizeof(glm::vec2))));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &_vao);
		glDeleteVertexArrays(1, &_ebo);
		glDeleteBuffers(1, &_vbo);
	}

	const uint32_t& OpenGLVertexArray::GetVAO() const
	{
		return _vao;
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	}

	void OpenGLVertexArray::SetData(int64_t offset, int64_t size, const void* data) const
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	bool OpenGLVertexArray::IsIndexed() const
	{
		return indices.size() > 0;
	}

	const int32_t& OpenGLVertexArray::GetElementsCount() const
	{
		return IsIndexed()
			? indices.size()
			: vertices.size();
	}
}
