#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

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
