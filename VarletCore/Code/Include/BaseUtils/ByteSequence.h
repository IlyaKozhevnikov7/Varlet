#pragma once

#include "Common.h"

namespace Varlet::Core
{
	class CORE_API ByteSequence
	{
	private:

		int8_t* _data;
		size_t _size;
		size_t _position;

	public:

		virtual ~ByteSequence() = default;

		ByteSequence(void* data, size_t size);

		ByteSequence(const ByteSequence&) = default;

		ByteSequence(ByteSequence&&) = default;

		size_t GetSize() const;

		size_t GetPosition() const;

	protected:

		void* GetData();

		void MovePosition(size_t size);
	};
}