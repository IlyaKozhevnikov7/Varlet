#pragma once

#include "VarletCore.h"

enum class Type : uint8_t;

namespace Varlet
{
	struct Vertex
	{
	public:

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinate;
		glm::vec3 tangent;
	};

	struct LayoutBinding final
	{
	public:

		Type type;
		uint64_t size;
		void* data;
	};

	struct VertexArrayData
	{
	public:

		std::vector<LayoutBinding> bindings;
		std::vector<struct Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	class CORE_API VertexArray
	{
	public:

		std::vector<struct Vertex> vertices;
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
