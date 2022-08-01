#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class CORE_API UniformBuffer
	{
	protected:

		uint32_t _id;

	public:

		UniformBuffer() = default;

		virtual ~UniformBuffer() = default;

		virtual void Bind() const = 0;

		virtual void SetData(int64_t offset, int64_t size, const void* data) const = 0;
	};
}
