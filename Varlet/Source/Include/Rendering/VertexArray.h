#pragma once

#include "VarletCore.h"

namespace Varlet
{
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

		std::vector<Varlet::Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	class CORE_API VertexArray
	{
	public:

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

	public:

		VertexArray() = default;

		virtual ~VertexArray() = default;

		virtual const uint32_t& GetVAO() const = 0;

		virtual void Bind() const = 0;

		virtual void SetData(int64_t offset, int64_t size, const void* data) const = 0;

		virtual bool IsIndexed() const = 0;

		virtual const int32_t& GetElementsCount() const = 0;
	};
}
