#include "Serialization/Internal/Binary/BinarySerializeContexts.h"
#include "Serialization/Internal/Binary/BinarySerializer.h"

BinarySerializeContext::BinarySerializeContext(ByteArray& data) :
	_data(data)
{
}

void BinarySerializeContext::AddData(std::vector<int8_t>& data)
{
	_data.Write((void*)data.data(), data.size());
}

void BinarySerializeContext::AddData(std::vector<int8_t>&& data)
{
	_data.Write((void*)data.data(), data.size());
}

void BinarySerializeContext::AddData(void* data, size_t size)
{
	_data.Write(data, size);
}
