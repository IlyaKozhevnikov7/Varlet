#include "Serialization/BinaryFormatter.h"
#include "Serialization/Internal/Binary/BinarySerializer.h"
#include "Serialization/Internal/Binary/BinaryDeserializer.h"

std::vector<int8_t> BinaryFormatter::SerializeInternal(void* value, const Varlet::Core::Type* type)
{
	Varlet::Core::BinarySerializer serializer(value, type);
	return serializer.Serialize();
}

void* BinaryFormatter::DeserializeInternal(void* data, size_t size, std::vector<SharedResourceInfo>* sharedResources)
{
	Varlet::Core::BinaryDeserializer deserializer(data, size, sharedResources);
	return deserializer.Deserialize();
}
