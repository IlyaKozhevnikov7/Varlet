#include "BaseUtils/ByteWriter.h"

ByteWriter::ByteWriter(void* data, size_t size) :
	Varlet::Core::ByteSequence(data, size)
{
}

void ByteWriter::Write(void* value, size_t size)
{
	memcpy(GetData(), &value, size);
	MovePosition(size);
}

ByteArray::ByteArray(size_t capacity)
{
	_data.reserve(capacity);
}

size_t ByteArray::Size()
{
	return _data.size();
}

std::vector<int8_t> ByteArray::ExtractData()
{
	return std::move(_data);
}

void ByteArray::Write(void* value, size_t size)
{
	Write(value, size, _data.size());
}

void ByteArray::Write(void* value, size_t size, size_t position)
{
	//assert(position + size < _data.size());

	if (position + size <= _data.size())
	{
		memcpy(_data.data() + position, value, size);
	}
	else
	{
		const int8_t* insertValue = (int8_t*)value;
		_data.insert(_data.end(), insertValue, insertValue + size);
	}	
}

void ByteArray::SkipRange(size_t size)
{
	_data.insert(_data.end(), size, 0);
}
