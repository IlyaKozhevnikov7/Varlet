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
		std::vector<int32_t> indices;
	};

	class CORE_API VertexArray
	{
	public:

		VertexArray() = default;

		virtual ~VertexArray() = default;
	};
}
