#pragma once

#include <vector>
#include "GLM/glm.hpp"

namespace Varlet
{
	struct MeshData;
}

namespace Varlet::OpenGL
{
	struct VertexBuffer final
	{
	public:

		uint32_t vao;
		uint32_t vbo;
		uint32_t ebo;

		uint32_t verticesCount;

	public:

		VertexBuffer();

		VertexBuffer(const MeshData& data);

		void Regenerate(const MeshData& data);

		static void Free(const VertexBuffer& buffer);

	private:

		void Initialize(const MeshData& data);

		void SetBufferBinding(const void* data, int32_t binding, int64_t elementSize, int64_t elementCount, int64_t& offset);

		int64_t CalculateBufferSize(const MeshData& data) const;
	};
}
