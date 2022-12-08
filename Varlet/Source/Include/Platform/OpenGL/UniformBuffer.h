#pragma once

#include <stdint.h>
#include <glad/glad.h>

namespace Varlet::OpenGL
{
	struct UniformBuffer final
	{
	private:

		static uint32_t bindingCounter;

	public:

		uint32_t binding;
		GLuint id;

		UniformBuffer() = default;

		UniformBuffer(const GLsizeiptr& size);

		void Bind() const;

		void SetData(const GLintptr& offset, const GLsizeiptr& size, const void* data) const;
	};
}
