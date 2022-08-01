#include "OpenGLUniformBuffer.h"
#include "glad/glad.h"

namespace Varlet
{
	uint32_t OpenGLUniformBuffer::_bindingPointCounter = 0;

	OpenGLUniformBuffer::OpenGLUniformBuffer(const int64_t& size)
	{
		_bindingPoint = _bindingPointCounter++;

		glGenBuffers(1, &_id);

		glBindBuffer(GL_UNIFORM_BUFFER, _id);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, _bindingPoint, _id, 0, size);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &_id);
	}

	void OpenGLUniformBuffer::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, _id);
	}

	void OpenGLUniformBuffer::SetData(int64_t offset, int64_t size, const void* data) const
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}
}
