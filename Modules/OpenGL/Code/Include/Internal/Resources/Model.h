#pragma once

#include "OpenGLCore.h"
#include "glad/glad.h"

namespace Varlet::Graphics
{
	enum class ShaderDataType : uint8_t;
	struct VertexAttributeInfo;
	struct SubMeshInfo;
	struct ModelCreateInfo;

	namespace OpenGL
	{
		struct Mesh;

		struct Model final
		{
		public:

			std::vector<Mesh> meshes;

		public:

			Model() = default;

			Model(const ModelCreateInfo& createInfo);

			Model& operator=(Model&& other) noexcept;
		};

		struct Mesh final
		{
		public:

			GLuint vao = 0;
			GLuint vbo = 0;
			GLuint ebo = 0;
			GLsizei verticesCount = 0;

		public:

			Mesh(const Mesh&) = default;

			Mesh(Mesh&& other) noexcept;

			Mesh(const std::vector<VertexAttributeInfo>& layout, SubMeshInfo meshInfo);

			~Mesh();

		private:

			GLsizeiptr CalculateBufferSize(const std::vector<VertexAttributeInfo>& layout, uint32_t verticesCount) const;

			GLsizeiptr GetTypeSize(ShaderDataType type) const;

			GLint GetValueComponentCount(ShaderDataType type) const;
		};
	}
}