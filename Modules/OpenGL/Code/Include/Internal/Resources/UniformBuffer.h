#pragma once

#include "OpenGLCore.h"
#include "glad/glad.h"

namespace Varlet::Graphics
{
	struct UniformBufferCreateInfo;
	enum class BufferUsage : uint8_t;

	namespace OpenGL
	{
		struct UniformBuffer final
		{
		private:

			static std::unordered_map<std::string, GLuint> _bufferBindings;

		public:

			GLuint id;
			GLenum usageType;

		public:

			static GLuint GetBufferBinding(const std::string& name);

			UniformBuffer() = default;

			UniformBuffer(const UniformBufferCreateInfo& createInfo);

			~UniformBuffer();

			UniformBuffer& operator=(UniformBuffer&& other) noexcept;

			void UpdateData(void* data, GLsizeiptr size, GLintptr offset);

		private:

			GLenum ConvertUsageToGLType(BufferUsage usage);
		};
	}
}