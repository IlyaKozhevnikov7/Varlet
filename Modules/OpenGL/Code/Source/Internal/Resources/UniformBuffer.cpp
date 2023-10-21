#include "Internal/Resources/UniformBuffer.h"

#include "GraphicsBaseTypes.h"

namespace Varlet::Graphics::OpenGL
{
	std::unordered_map<std::string, GLuint> UniformBuffer::_bufferBindings;

	GLuint UniformBuffer::GetBufferBinding(const std::string& name)
	{
		if (_bufferBindings.contains(name) == false)
			_bufferBindings[name] = _bufferBindings.size();

		return _bufferBindings[name];
	}

	UniformBuffer::UniformBuffer(const UniformBufferCreateInfo& createInfo)
	{
		assert(createInfo.bindingName.empty() == false);

		const GLuint binding = GetBufferBinding(createInfo.bindingName);		
		usageType = ConvertUsageToGLType(createInfo.usage);

		glGenBuffers(1, &id);

		glBindBuffer(usageType, id);
		glBufferData(usageType, createInfo.size, createInfo.initialData, GL_STREAM_DRAW);

		glBindBufferBase(usageType, binding, id);
		glBindBuffer(usageType, 0);
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &id);
	}

	UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
	{
		id = other.id;
		usageType = other.usageType;

		other.id = 0;

		return *this;
	}			

	void UniformBuffer::UpdateData(void* data, GLsizeiptr size, GLintptr offset)
	{
		glBindBuffer(usageType, id);
		glBufferSubData(usageType, offset, size, data);
		glBindBuffer(usageType, 0);
	}

	GLenum UniformBuffer::ConvertUsageToGLType(BufferUsage usage)
	{
		switch (usage)
		{
		case Varlet::Graphics::BufferUsage::Uniform:	return GL_UNIFORM_BUFFER;
		case Varlet::Graphics::BufferUsage::Storage:	return GL_SHADER_STORAGE_BUFFER;
		}

		VARLET_ERROR("UniformBuffer support only Uniform of Storage usage");
		return 0;
	}
}
