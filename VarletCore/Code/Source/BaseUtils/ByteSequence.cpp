#include "BaseUtils/ByteSequence.h"

namespace Varlet::Core
{
	ByteSequence::ByteSequence(void* data, size_t size) :
		_data(std::_Bit_cast<int8_t*>(data)),
		_size(size),
		_position(0)
	{
	}

	size_t ByteSequence::GetSize() const
	{
		return _size;
	}

	size_t ByteSequence::GetPosition() const
	{
		return _position;
	}

	void* ByteSequence::GetData()
	{
		return _data + _position;
	}

	void ByteSequence::MovePosition(size_t size)
	{
		assert(_position + size <= _size);
		_position += size;
	}
}
