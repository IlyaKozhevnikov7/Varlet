#pragma once

#include "Serialization/SerializationUtils.h"

namespace Varlet::Core
{
	using SharedObjId = std::vector<int8_t>;

	namespace SerializeFlagBit
	{
		enum SerializeFlagBit : uint8_t
		{
			WithDependencies = BIT<1>,
			SkipDependencyLinking = BIT<2>,
			WithFieldMeta = BIT<3>
		};
	}

	namespace DeserializeFlagBit
	{
		enum DeserializeFlagBit : uint8_t
		{
			WithFieldMeta = BIT<1>
		};
	}

	struct ValueToSerialize final
	{
		int8_t* address;
		const Type* type;
		uint32_t objectId;
	};

	struct SerializationContext final
	{
		int32_t currentIndex = -1;
		std::vector<ValueToSerialize> dependencies;
		BitMask<SerializeFlagBit::SerializeFlagBit> flags;

		int8_t* currentProcessedData;
		const Type* currentProcessedType;
		BitMask<FieldMetaFlags> currentProcessedDataFlags;

		uint32_t GetDependenceId(int8_t* address, const Type* type)
		{
			for (auto& dependence : dependencies)
				if (dependence.address == address)
					return dependence.objectId;

			const ValueToSerialize toSeriazlize = { address, type, dependencies.size() + 1 };
			dependencies.push_back(toSeriazlize);
			return toSeriazlize.objectId;
		}
		
		bool MoveNext()
		{
			if (++currentIndex >= dependencies.size())
				return false;

			currentProcessedData = dependencies[currentIndex].address;
			currentProcessedType = dependencies[currentIndex].type;
			currentProcessedDataFlags = 0;

			return true;
		}

		const ValueToSerialize& GetCurrentDependence()
		{
			return dependencies[currentIndex];
		}
	};

	struct CORE_API SerializeInfo final
	{
		std::vector<int8_t> data;

	public:

		void AddData(const int8_t* value, int32_t size)
		{
			if (value == nullptr)
				data.insert(data.end(), size, 0);
			else
				data.insert(data.end(), value, value + size);
		}

		template<typename T>
		void AddData(const T& value)
		{
			const int8_t* valuePtr = std::bit_cast<const int8_t*>(&value);
			data.insert(data.end(), valuePtr, valuePtr + sizeof(T));
		}

		void AddData(const std::vector<int8_t>& value)
		{
			data.insert(data.end(), value.begin(), value.end());
		}

		void AddData(const std::vector<int8_t>&& value)
		{
			data.insert(data.end(), value.begin(), value.end());
		}

		void AddSharedId(const std::vector<int8_t>& value)
		{
			uint32_t size = value.size();

			AddData(size);
			AddData(value.data(), size);
		}
	};

	struct SharedPtrInfo
	{
		const Varlet::Core::Type* type;
		void** ref;
	};

	struct SharedData final
	{
		SharedObjId id;
		std::vector<SharedPtrInfo> refs;
 	};

	struct DeserializeInfo final
	{
	private:

		struct DependenciesSet final
		{
			void* instance;
			std::vector<void*> refs;
		};

		std::vector<int8_t> _data;
		uint64_t _offset;

		std::unordered_map<uint32_t, DependenciesSet> _dependenceSets;
		std::vector<SharedData> _sharedData;

	public:

		int8_t* currentProcessedData = nullptr;
		const Type* currentProcessedType = nullptr;

		DeserializeInfo(std::vector<int8_t>&& data)
		{
			_data = std::move(data);
			_offset = 0;
		}

		template<typename T, auto TSize = sizeof(T)>
		T Read()
		{
			static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T> || std::is_pointer_v<T> || std::is_reference_v<T>);

			auto current = GetCurrent();
			AddOffset(TSize);

			return *std::bit_cast<T*>(current);
		}

		CORE_API void ReadPointer(void* destination)
		{
			uint32_t id = *std::bit_cast<uint32_t*>(GetCurrent());
			_dependenceSets[id].refs.push_back(destination);

			AddOffset(sizeof(uint32_t));
		}	

		CORE_API void ReadTo(void* destination, int32_t size)
		{
			memcpy(destination, GetCurrent(), size);
			AddOffset(size);
		}

		CORE_API std::vector<int8_t> ReadSharedData()
		{
			std::vector<int8_t> sharedObjId;
			uint32_t size = Read<uint32_t>();

			if (size > 0)
			{
				sharedObjId.resize(size);

				ReadTo(&sharedObjId[0], size);
				AddSharedData(currentProcessedType, sharedObjId, std::bit_cast<void**>(currentProcessedData));
			}

			return sharedObjId;
		}

		void AddDependency(uint32_t id, void* ref)
		{
			assert(_dependenceSets[id].instance == nullptr);

			_dependenceSets[id].instance = ref;
		}

		void AddSharedData(const Type* ptrType, SharedObjId& id, void* ref)
		{
			auto it = std::find_if(_sharedData.begin(), _sharedData.end(), [&id](SharedData& data)
				{
					return id.size() == data.id.size() && std::equal(id.begin(), id.end(), data.id.begin());
				});

			if (it != _sharedData.end())
			{
				(*it).refs.push_back({ ptrType, std::bit_cast<void**>(ref) });
				return;
			}

			_sharedData.push_back({ id, { { ptrType, std::bit_cast<void**>(ref) } } });
		}

		bool HasData() const
		{
			return _offset < _data.size();
		}

		void ResolveDependencies()
		{
			for (auto& set : _dependenceSets)
				for (auto& ref : set.second.refs)
					*std::bit_cast<void**>(ref) = set.second.instance;
		}

		std::vector<SharedData> GetSharedData()
		{
			return _sharedData;
		}

	private:
	
		void AddOffset(int32_t size)
		{
			_offset += size;
			assert(_offset <= _data.size());
		}

		int8_t* GetCurrent()
		{
			return &_data[_offset];
		}
	};

	class CORE_API BinaryFormatter final
	{
	public:

		template<typename T>
		static std::vector<int8_t> Serialize(T* value, BitMask<SerializeFlagBit::SerializeFlagBit> flags = SerializeFlagBit::WithDependencies)
		{
			assert(value);
			const Type* type = value->GetType();

			SerializationContext context;
			context.flags = flags;
			context.GetDependenceId(std::bit_cast<int8_t*>(value->GetObjectTop()), type);

			SerializeInfo info;
			
			SerializeAll(info, context);

			return info.data;
		}

		template<typename T>
		static T* Deserialize(std::vector<int8_t>& data, std::vector<SharedData>& sharedData)
		{
			assert(data.size() > 0);

			DeserializeInfo info(std::move(data));

			int8_t* commonInstance = nullptr;
			const Type* type = nullptr;

			while (info.HasData())
			{
				uint32_t typeId = info.Read<uint32_t>();

				info.currentProcessedType = Type::GetType(typeId);
				assert(info.currentProcessedType);

				int8_t* instance = std::bit_cast<int8_t*>(info.currentProcessedType->CreateInstance());
				info.currentProcessedData = instance;

				if (commonInstance == nullptr)
				{
					type = info.currentProcessedType;
					assert(type->IsA(T));

					commonInstance = instance;
				}

				uint32_t dependenceId = info.Read<uint32_t>();
				info.AddDependency(dependenceId, instance);

				DeserializeValue(info);
			}

			info.ResolveDependencies();
			sharedData = info.GetSharedData();

			return CastMemory<T>(type, commonInstance);
		}

		static void* Deserialize(std::vector<int8_t>& data, std::vector<SharedData>& sharedData)
		{
			assert(data.size() > 0);

			DeserializeInfo info(std::move(data));

			int8_t* commonInstance = nullptr;
			const Type* type = nullptr;

			while (info.HasData())
			{
				uint32_t typeId = info.Read<uint32_t>();

				info.currentProcessedType = Type::GetType(typeId);
				assert(info.currentProcessedType);

				int8_t* instance = std::bit_cast<int8_t*>(info.currentProcessedType->CreateInstance());
				info.currentProcessedData = instance;

				if (commonInstance == nullptr)
				{
					type = info.currentProcessedType;
					commonInstance = instance;
				}

				uint32_t dependenceId = info.Read<uint32_t>();
				info.AddDependency(dependenceId, instance);

				DeserializeValue(info);
			}

			info.ResolveDependencies();
			sharedData = info.GetSharedData();

			return commonInstance;
		}

	private:

		static void SerializeAll(SerializeInfo& info, SerializationContext& context);

		static void SerializeDependence(SerializeInfo& info, SerializationContext& context);

		static void SerializeValue(SerializeInfo& info, SerializationContext& context);

		static void SerializePointer(SerializeInfo& info, SerializationContext& context);

		static void SerializeArray(SerializeInfo& info, SerializationContext& context);

		static void DeserializeValue(DeserializeInfo& info);

		static void DeserializePointer(DeserializeInfo& info);

		static void DeserializeArray(DeserializeInfo& info)
		{
			auto arrrayType = dynamic_cast<const IArrayType*>(info.currentProcessedType);
			info.currentProcessedType = Type::GetType(arrrayType->tId);
			assert(info.currentProcessedType);

			const bool isPointer = arrrayType->tFlags.Has(FieldMetaFlags::Pointer);

			uint32_t size = info.Read<uint32_t>();

			const uint32_t actualElementSize = isPointer
				? sizeof(size_t)
				: info.currentProcessedType->size;

			const uint32_t serializedElementOffset = isPointer
				? sizeof(uint32_t) // dependence id is uint32_t
				: info.currentProcessedType->size;

			arrrayType->Resize(info.currentProcessedData, size);
			auto actualArrayData = arrrayType->GetData(info.currentProcessedData);

			for (uint32_t i = 0; i < size; i++)
			{
				info.currentProcessedData = actualArrayData + (actualElementSize * i);

				if (isPointer)
				{
					DeserializePointer(info);
				}
				else
				{
					DeserializeValue(info);
				}
			}
		}

		static bool IsPrimetiveType(const uint32_t& id);

		static bool IsString(const uint32_t& id);

		static int32_t HashName(const char* name);
	};
}