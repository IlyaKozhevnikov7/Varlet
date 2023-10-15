#pragma once

#include "Common.h"
#include "Reflection/TemplateType.h"
#include "BaseUtils/ByteWriter.h"

namespace Varlet::Core
{
	class FieldDataKey final
	{
	public:

		uint32_t nameHash;
		uint32_t region;

		FieldDataKey() = default;
		FieldDataKey(const FieldDataKey&) = default;

		FieldDataKey(uint32_t nameHash, uint32_t region);

		bool operator==(const FieldDataKey& other) const;
	};

	class BinarySerializer final
	{
	private:

		struct RegionInfo
		{
			uint32_t startPosition;
		};

		struct InstanceInfo final
		{
			int8_t* value = nullptr;
			const Type* type = nullptr;
		};

	private:

		ByteArray _data;
		std::unordered_map<void*, uint32_t> _objectIds;
		std::stack<size_t> _regionStack;

		std::vector<InstanceInfo> _shouldSerializeInstances;
		int32_t _currentInstanceIndex = -1;

		const Type* _currentType = nullptr;
		int8_t* _currentInstanceData = nullptr;

	public:

		BinarySerializer(void* value, const Varlet::Core::Type* type);

		std::vector<int8_t> Serialize();

	private:

		void SerializeInstance();

		void SeriazeliByType(int8_t* value, const Type* type);

		void SerializeField(int8_t* value, const FieldInfo& info);

		void SerializePointer(void* pointer, const Type* type);

		bool NextInstance();

		void BeginSerializeInstance();

		void EndSerializeInstance();

		void BeginSerializeField(const FieldInfo& fieldInfo);

		void EndSerializeField();

		void AddData(void* value, size_t size);

		template<typename T>
		void AddData(T& value)
		{
			_data.Write((void*)&value, sizeof(T));
		}

		template<typename T>
		void AddData(T&& value)
		{
			_data.Write(std::forward<T>(value));
		}

		void AddShouldSerializeInstance(void* instance, const Type* type);

		uint32_t GetId(void* object, const Varlet::Core::Type* type);

		void PushRegion();

		void PopRegion();

		uint32_t HashName(const char* name);

		// TEMP for std
		void SerializeArray(void* value, const IArrayType* type);
	};
}