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

	public:

		OpenGLVertexArray(const VertexArrayData& data);

		~OpenGLVertexArray() override;

		const uint32_t& GetVAO() const override;

		void Bind() const override;

		void SetData(int64_t offset, int64_t size, const void* data) const override;

		bool IsIndexed() const override;

		const int32_t& GetElementsCount() const override;
	};
}
