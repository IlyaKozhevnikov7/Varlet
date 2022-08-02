#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class CORE_API OpenGLUniformBuffer final : public UniformBuffer
	{
		friend class OpengGLShader;

	private:

		static uint32_t _bindingPointCounter;

		uint32_t _bindingPoint;

	public:

		OpenGLUniformBuffer(const int64_t& size);

		~OpenGLUniformBuffer() override;

		void Bind() const override;

		void SetData(int64_t offset, int64_t size, const void* data) const override;
	};
}
