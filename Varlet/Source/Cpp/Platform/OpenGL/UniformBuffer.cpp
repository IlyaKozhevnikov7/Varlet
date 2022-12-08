#include "OpenGL/UniformBuffer.h"

namespace Varlet::OpenGL
{
	uint32_t UniformBuffer::bindingCounter;

	UniformBuffer::UniformBuffer(const GLsizeiptr& size)
	{
		binding = bindingCounter++;

		glGenBuffers(1, &id);

		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, binding, id, 0, size);
	}

	void UniformBuffer::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, id);
	}

	void UniformBuffer::SetData(const GLintptr& offset, const GLsizeiptr& size, const void* data) const
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}
}
