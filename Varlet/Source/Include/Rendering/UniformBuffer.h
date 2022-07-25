#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class CORE_API UniformBuffer
	{
	protected:

		uint32_t _id;

	public:

		UniformBuffer(const int64_t& size);

		virtual void Bind() const = 0;

		virtual void SetData(int64_t offset, int64_t size, const void* data) const = 0;
	};
}
