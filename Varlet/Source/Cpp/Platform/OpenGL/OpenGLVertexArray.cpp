#include "OpenGLVertexArray.h"

namespace Varlet
{
	OpenGLVertexArray::OpenGLVertexArray(const VertexArrayData& data)
	{
		_vertices = data.vertices;
		_indices = data.indices;

		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_ebo);
		glGenBuffers(1, &_vbo);

		glBindVertexArray(_vao);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32_t), &_indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

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

	bool OpenGLVertexArray::IsIndexed() const
	{
		return _indices.size() > 0;
	}

	const int32_t& OpenGLVertexArray::GetElementsCount() const
	{
		return IsIndexed()
			? _indices.size()
			: _vertices.size();
	}
}
