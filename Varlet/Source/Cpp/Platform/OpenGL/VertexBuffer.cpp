#include "OpenGL/VertexBuffer.h"
#include <glad/glad.h>

// copy from Varlet Mesh.h
namespace Varlet
{
	struct MeshData final
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> tangents;
		std::vector<uint32_t> indices;
	};
}

namespace Varlet::OpenGL
{
	VertexBuffer::VertexBuffer() :
		vao(0),
		vbo(0),
		ebo(0)
	{
	}

	VertexBuffer::VertexBuffer(const MeshData& data) 
		: VertexBuffer()
	{
		Initialize(data);
	}

	void VertexBuffer::Regenerate(const MeshData& data)
	{
		Free(*this);
		Initialize(data);
	}

	void VertexBuffer::Free(const VertexBuffer& buffer)
	{
		glDeleteVertexArrays(1, &buffer.vao);
		glDeleteVertexArrays(1, &buffer.ebo);
		glDeleteBuffers(1, &buffer.vbo);
	}

	void VertexBuffer::Initialize(const MeshData& data)
	{
#define NULL_IF_EMPTY(Data) Data.size() > 0 ? &Data[0] : nullptr

		const int64_t vectexCount = data.positions.size();
		const int64_t bufferSize = CalculateBufferSize(data);
		int64_t offset = 0;

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);

		SetBufferBinding(NULL_IF_EMPTY(data.positions),	0, sizeof(glm::vec3), vectexCount, offset);
		SetBufferBinding(NULL_IF_EMPTY(data.normals),	1, sizeof(glm::vec3), vectexCount, offset);
		SetBufferBinding(NULL_IF_EMPTY(data.uvs),		2, sizeof(glm::vec2), vectexCount, offset);
		SetBufferBinding(NULL_IF_EMPTY(data.tangents),	3, sizeof(glm::vec3), vectexCount, offset);

		if (data.indices.size() > 0)
		{
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), &data.indices[0], GL_STATIC_DRAW);

			verticesCount = data.indices.size();
		}
		else
		{
			ebo = 0;
			verticesCount = data.positions.size();
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#undef NULL_IF_EMPTY(Data)
	}

	void VertexBuffer::SetBufferBinding(const void* data, int32_t binding, int64_t elementSize, int64_t elementCount, int64_t& offset)
	{
		if (data == nullptr)
			return;

		const int32_t valueComponentCount = elementSize / 4;

		glBufferSubData(GL_ARRAY_BUFFER, offset, elementCount * elementSize, data);

		glEnableVertexAttribArray(binding);
		glVertexAttribPointer(binding, valueComponentCount, GL_FLOAT, GL_FALSE, elementSize, (void*)(offset));

		offset += elementCount * elementSize;
	}

	int64_t VertexBuffer::CalculateBufferSize(const MeshData& data) const
	{
		return data.positions.size() * sizeof(data.positions[0])
			+ data.normals.size() * sizeof(data.normals[0])
			+ data.uvs.size() * sizeof(data.uvs[0])
			+ data.tangents.size() * sizeof(data.tangents[0]);
	}
}
