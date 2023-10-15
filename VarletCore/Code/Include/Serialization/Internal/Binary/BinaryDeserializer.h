#pragma once

#include "Common.h"
#include "Serialization/BinaryFormatter.h"
#include "Reflection/TemplateType.h"
#include "BaseUtils/ByteReader.h"

struct SharedResourceInfo;

namespace Varlet::Core
{
	struct DeserializedInstanceHeader final
	{
		constexpr static uint32_t SIZE = 12;

		uint32_t typeId;
		uint32_t instanceId;
		uint32_t regionSize;
	};

	struct DeserializedFieldInfo final
	{
		constexpr static uint32_t SIZE = 13;

		uint32_t typeId;
		BitMask<FieldMetaFlags> flags;
		uint32_t nameHashed;
		uint32_t regionLevel; /* Defines nesting level */
		uint32_t regionSize;
		void* value = nullptr;
	};

	class BinaryDeserializeContext final : public IDeserializeContext
	{
	private:

		ByteReader _reader;

	public:

		BinaryDeserializeContext(void* data, size_t size);

		std::vector<int8_t> ReadAll() override;

		uint32_t GetDataSize() override;

		void* Read(size_t size) override;
	};

	class BinaryDeserializer final
	{
		friend BinaryDeserializeContext;

	private:

		ByteReader _reader;
		uint32_t _currentRegionLevel = 0;

		const Type* _currentInstanceType = nullptr;
		DeserializedInstanceHeader _currentInstanceHeader;
		std::vector<DeserializedFieldInfo> _currentInstanceValues;

		std::unordered_map<uint32_t, void*> _instances;
		std::unordered_map<uint32_t, std::vector<void**>> _references;

		std::vector<SharedResourceInfo>* _sharedResources;

	public:

		BinaryDeserializer(void* data, size_t size, std::vector<SharedResourceInfo>* sharedResources);

		void* Deserialize();

	private:

		void DeserializeInstance();

		void DeserializeField();

		void RestoreInstance();

		void RestoreByType(int8_t* restoreValue, const Type* type);

		void RestorePointer(void* pointer, const Type* pointerType, void* value, uint32_t valueSize);

		void RestoreArray(int8_t* value, const DeserializedFieldInfo* fieldInfo, const IArrayType* arrayType);

		void ResolveDependencies();

		DeserializedFieldInfo* GetValue(const FieldInfo& fieldInfo);
	};
}
