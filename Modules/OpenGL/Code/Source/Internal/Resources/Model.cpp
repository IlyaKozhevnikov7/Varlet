#include "Internal/Resources/Model.h"
#include "GraphicsBaseTypes.h"

namespace Varlet::Graphics::OpenGL
{
	Model::Model(const ModelCreateInfo& createInfo)
	{
		assert(createInfo.subMeshInfos.size() > 0);

		meshes.reserve(createInfo.subMeshInfos.size());

		for (auto& subMeshInfo : createInfo.subMeshInfos)
			meshes.push_back(Mesh(createInfo.vertexLayout, subMeshInfo));
	}

	Model& Model::operator=(Model&& other) noexcept
	{
		meshes = std::move(other.meshes);

		return *this;
	}

	Mesh::Mesh(const std::vector<VertexAttributeInfo>& layout, SubMeshInfo meshInfo)
	{
		assert(meshInfo.vertexData.size() > 0);

		const GLsizeiptr bufferSize = CalculateBufferSize(layout, meshInfo.verticesCount);

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);

		GLintptr offset = 0;
		for (auto& attribute : layout)
		{
			const GLsizeiptr elementSize = GetTypeSize(attribute.type);
			const GLsizeiptr size = elementSize * meshInfo.verticesCount;
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, meshInfo.vertexData.data() + offset);

			glEnableVertexAttribArray(attribute.location);
			glVertexAttribPointer(attribute.location, GetValueComponentCount(attribute.type), GL_FLOAT, GL_FALSE, elementSize, (void*)(offset));

			offset += size;
		}

		if (meshInfo.indices.size() > 0)
		{
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshInfo.indices.size() * sizeof(uint32_t), meshInfo.indices.data(), GL_STATIC_DRAW);

			verticesCount = meshInfo.indices.size();
		}
		else
		{
			ebo = 0;
			verticesCount = meshInfo.verticesCount;
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	Mesh::Mesh(Mesh&& other) noexcept
	{
		vao = other.vao;
		vbo = other.vbo;
		ebo = other.ebo;
		verticesCount = other.verticesCount;

		other.vao = 0;
		other.vbo = 0;
		other.ebo = 0;
		other.verticesCount = 0;
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}

	GLsizeiptr Mesh::CalculateBufferSize(const std::vector<VertexAttributeInfo>& layout, uint32_t verticesCount) const
	{
		GLsizeiptr vertexSize = 0;

		for (auto& attribute : layout)
			vertexSize += GetTypeSize(attribute.type);

		return vertexSize * verticesCount;
	}

	GLsizeiptr Mesh::GetTypeSize(ShaderDataType type) const
	{
		switch (type)
		{
		case ShaderDataType::Float:	return sizeof(float);
		case ShaderDataType::Vec2:	return sizeof(glm::vec2);
		case ShaderDataType::Vec3:	return sizeof(glm::vec3);
		}
	}

	GLint Mesh::GetValueComponentCount(ShaderDataType type) const
	{
		switch (type)
		{
		case ShaderDataType::Float:	return 1;
		case ShaderDataType::Vec2:	return 2;
		case ShaderDataType::Vec3:	return 3;
		}
	}
}