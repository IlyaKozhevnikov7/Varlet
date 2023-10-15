#include "Serialization/Internal/Binary/BinarySerializer.h"
#include "Serialization/Internal/Binary/BinarySerializeContexts.h"
#include "Serialization/SerializationUtils.h"

namespace Varlet::Core
{
	FieldDataKey::FieldDataKey(uint32_t nameHash, uint32_t region) :
		nameHash(nameHash),
		region(region)
	{
	}

	bool FieldDataKey::operator==(const FieldDataKey& other) const
	{
		return nameHash == other.nameHash && region == other.region;
	}

	BinarySerializer::BinarySerializer(void* value, const Varlet::Core::Type* type) :
		_data(1024)
	{
		AddShouldSerializeInstance(value, type);
	}

	std::vector<int8_t> BinarySerializer::Serialize()
	{
		while (NextInstance())
		{
			SerializeInstance();
		}

		return _data.ExtractData();
	}

	void BinarySerializer::SerializeInstance()
	{
		BeginSerializeInstance();
		SeriazeliByType(_currentInstanceData, _currentType);
		EndSerializeInstance();
	}

	void BinarySerializer::SeriazeliByType(int8_t* value, const Type* type)
	{
		if (type->IsA(ICustomSerializable))
		{
			auto customSerializable = CastMemory<ICustomSerializable>(type, value);

			BinarySerializeContext context(_data);
			customSerializable->Serialize(&context);
		}
		else
		{
			for (const auto& fieldInfo : type->fieldInfos)
				SerializeField(value + fieldInfo.offset, fieldInfo);	
		}
	}

	void BinarySerializer::SerializeField(int8_t* value, const FieldInfo& info)
	{
		if (info.flags.Has(FieldMetaFlags::Static))
			return;

		BeginSerializeField(info);

		const auto fieldType = Type::GetType(info.typeId);

		if (info.flags.Has(FieldMetaFlags::Pointer))
		{
			SerializePointer(value, fieldType);
		}
		else if (fieldType->IsA(ICustomSerializable))
		{
			ICustomSerializable* customSerializable = CastMemory<ICustomSerializable>(fieldType, value);

			BinarySerializeContext context(_data);
			customSerializable->Serialize(&context);
		}
		else if (Type::IsPrimetive(info.typeId))
		{
			AddData(value, fieldType->size);
		}
		else
		{
			if (fieldType->module == nullptr)
			{
				if (auto arrayType = dynamic_cast<const IArrayType*>(fieldType))
					SerializeArray(value, arrayType);
			}
			else
			{
				SeriazeliByType(value, fieldType);
			}
		}

		EndSerializeField();
	}

	void BinarySerializer::SerializePointer(void* pointer, const Type* type)
	{
		int8_t* pointerValue = *(int8_t**)pointer;

		if (pointerValue != nullptr)
		{
			const auto actualType = type->GetActualType(pointerValue);

			if (type->IsA(ISharedSerializable))
			{
				const auto sharedSerializable = CastMemory<ISharedSerializable>(actualType, pointerValue);
				auto sharedId = sharedSerializable->GetSharedId();
				
				AddData(sharedId.data(), sharedId.size());
			}
			else
			{
				AddData(GetId(pointerValue, actualType));
				AddShouldSerializeInstance(pointerValue, actualType);
			}
		}
		else
		{
			AddData<uint32_t>(0);
		}
	}

	bool BinarySerializer::NextInstance()
	{
		if (++_currentInstanceIndex < _shouldSerializeInstances.size())
		{
			_currentType = _shouldSerializeInstances[_currentInstanceIndex].type;
			_currentInstanceData = _shouldSerializeInstances[_currentInstanceIndex].value;
			return true;
		}

		return false;
	}

	void BinarySerializer::BeginSerializeInstance()
	{
		_data.Write(_currentType->id);
		_data.Write(GetId(_currentInstanceData, _currentType));
		PushRegion();
	}

	void BinarySerializer::EndSerializeInstance()
	{
		PopRegion();
	}

	void BinarySerializer::BeginSerializeField(const FieldInfo& fieldInfo)
	{
		_data.Write(fieldInfo.typeId);
		_data.Write(fieldInfo.flags.Get());
		_data.Write(HashName(fieldInfo.name.c_str()));

		PushRegion();
	}

	void BinarySerializer::EndSerializeField()
	{
		PopRegion();
	}

	void BinarySerializer::AddData(void* value, size_t size)
	{
		_data.Write(value, size);
	}

	void BinarySerializer::AddShouldSerializeInstance(void* instance, const Type* type)
	{
		const auto it = std::find_if(_shouldSerializeInstances.begin(), _shouldSerializeInstances.end(), [instance, type](InstanceInfo& info)
			{
				return info.value == instance && info.type == type;
			});

		if (it == _shouldSerializeInstances.end())
			_shouldSerializeInstances.push_back({ std::bit_cast<int8_t*>(instance), type });
	}

	uint32_t BinarySerializer::GetId(void* object, const Varlet::Core::Type* type)
	{
		assert(type);

		if (object == nullptr)
			return 0;

		if (_objectIds.contains(object) == false)
			_objectIds[object] = _objectIds.size() + 1;

		return _objectIds[object];
	}

	void BinarySerializer::PushRegion()
	{
		_regionStack.push(_data.Size());
		_data.SkipRange(sizeof(uint32_t));
	}

	void BinarySerializer::PopRegion()
	{
		uint32_t regionPosition = _regionStack.top();
		const uint32_t regionSize = _data.Size() - regionPosition - sizeof(uint32_t);

		_data.Write(regionSize, regionPosition);
		_regionStack.pop();
	}

	uint32_t BinarySerializer::HashName(const char* name)
	{
		uint32_t hash = 5381;
		int32_t c;

		while (c = *name++)
			hash = ((hash << 5) + hash) + c;

		return hash;
	}

	void BinarySerializer::SerializeArray(void* value, const IArrayType* type)
	{
		const bool isPointerElements = type->tFlags.Has(FieldMetaFlags::Pointer);

		const uint32_t size = type->Size(value);
		int8_t* arrayData = type->GetData(value);
		const Type* elementType = Type::GetType(type->tId);

		if (elementType == nullptr)
		{
			// TODO: в таком случае значение региона будет 0, и при дисериализации нужно проверять значение региона, если он = 0, то скипать его, так как там нету данных
			VT_LOG(Error, "Can`t serialize array elememts %s", elementType->name.c_str());
			return;
		}

		uint32_t elementSize = isPointerElements
			? sizeof(size_t)
			: elementType->size;

		AddData(size);

		for (uint32_t i = 0; i < size; i++)
		{
			int8_t* currentElementData = arrayData + elementSize * i;

			if (isPointerElements)
			{
				PushRegion();
				SerializePointer(currentElementData, elementType);
				PopRegion();
			}
			else
			{
				AddData(currentElementData, elementSize);
			}
		}
	}
}
