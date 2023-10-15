#include "Serialization/Internal/Binary/BinaryDeserializer.h"
#include "Serialization/SerializationUtils.h"
#include "Serialization/BinaryFormatter.h"

namespace Varlet::Core
{
	BinaryDeserializeContext::BinaryDeserializeContext(void* data, size_t size) :
		_reader(data, size)
	{
	}

	std::vector<int8_t> BinaryDeserializeContext::ReadAll()
	{
		auto begin = (int8_t*)_reader.Read();
		return { begin, begin + _reader.GetSize() };
	}

	uint32_t BinaryDeserializeContext::GetDataSize()
	{
		return _reader.GetSize();
	}

	void* BinaryDeserializeContext::Read(size_t size)
	{
		return _reader.Read(size);
	}

	BinaryDeserializer::BinaryDeserializer(void* data, size_t size, std::vector<SharedResourceInfo>* sharedResources) :
		_reader(data, size)
	{
		_instances.reserve(5);
		_currentInstanceValues.reserve(5);
		_sharedResources = sharedResources;
	}

	void* BinaryDeserializer::Deserialize()
	{
		while (_reader.HasData())
		{
			DeserializeInstance();
			RestoreInstance();
		}

		ResolveDependencies();

		return _instances[1];
	}

	void BinaryDeserializer::DeserializeInstance()
	{
		assert(_currentRegionLevel == 0);

		_currentInstanceHeader = _reader.Read<DeserializedInstanceHeader>();
		assert(_instances.contains(_currentInstanceHeader.instanceId) == false);

		const size_t instanceRegionEnd = _reader.GetPosition() + _currentInstanceHeader.regionSize;

		if (_currentInstanceType = Varlet::Core::Type::GetType(_currentInstanceHeader.typeId))
		{
			if (_currentInstanceType->IsA(ICustomSerializable))
			{
				DeserializedFieldInfo info;
				info.typeId = 0;
				info.flags = 0;
				info.nameHashed = 0;
				info.regionSize = _currentInstanceHeader.regionSize;
				info.regionLevel = _currentRegionLevel;
				info.value = _reader.Read(_currentInstanceHeader.regionSize);

				_currentInstanceValues.push_back(info);
			}
			else
			{
				while (_reader.GetPosition() < instanceRegionEnd)
					DeserializeField();
			}
		}
		else
		{
			_reader.Read(_currentInstanceHeader.regionSize);
			VT_LOG(Error, "Failed to deserialize type %i", _currentInstanceHeader.typeId);
		}		
	}

	void BinaryDeserializer::DeserializeField()
	{
		DeserializedFieldInfo info;
		info.typeId = _reader.Read<uint32_t>();
		info.flags = _reader.Read<BitMask<FieldMetaFlags>>();
		info.nameHashed = _reader.Read<uint32_t>();
		info.regionSize = _reader.Read<uint32_t>();
		info.regionLevel = _currentRegionLevel;

		const auto type = Type::GetType(info.typeId);

		if (type == nullptr)
		{
			_reader.Read(info.regionSize);
			VT_LOG(Error, "Failed to deserialize type %i", info.typeId);
			return;
		}

		if (info.flags.Has(FieldMetaFlags::Pointer) || Type::IsPrimetive(info.typeId))
		{
			info.value = _reader.Read(info.regionSize);
		}
		else if (type->module == nullptr)
		{
			if (auto arrayType = dynamic_cast<const IArrayType*>(type))
				info.value = _reader.Read(info.regionSize);
		}
		else
		{
			++_currentRegionLevel;
			DeserializeField();
			--_currentRegionLevel;
		}

		_currentInstanceValues.push_back(info);
	}

	void BinaryDeserializer::RestoreInstance()
	{
		auto instance = (int8_t*)_currentInstanceType->CreateInstance();

		if (auto customSerializable = CastMemory<ICustomSerializable>(_currentInstanceType, instance))
		{
			assert(_currentInstanceValues.size() == 1);
			
			BinaryDeserializeContext context(_currentInstanceValues[0].value, (size_t)_currentInstanceValues[0].regionSize);
			
			if (customSerializable->Deserialize(&context) == false)
			{
				delete instance;
				instance = nullptr;
			}
		}
		else
		{
			RestoreByType(instance, _currentInstanceType);
		}

		_instances[_currentInstanceHeader.instanceId] = instance;
		_currentInstanceValues.clear();
	}

	void BinaryDeserializer::RestoreByType(int8_t* restoreValue, const Type* type)
	{
		for (const auto& fieldInfo : type->fieldInfos)
		{
			if (fieldInfo.flags.Has(FieldMetaFlags::Static))
				continue;

			const auto fieldValue = GetValue(fieldInfo);

			if (fieldValue == nullptr)
				continue;

			int8_t* fieldData = restoreValue + fieldInfo.offset;
			const auto fieldType = Type::GetType(fieldInfo.typeId);

			if (fieldInfo.flags.Has(FieldMetaFlags::Pointer))
			{
				RestorePointer(fieldData, fieldType, fieldValue->value, fieldValue->regionSize);
			}
			else if (fieldType->IsA(ICustomSerializable))
			{
				auto customSerializable = CastMemory<ICustomSerializable>(fieldType, fieldData);

				BinaryDeserializeContext context(fieldValue->value, fieldValue->regionSize);
				customSerializable->Deserialize(&context);
			}
			else if (Varlet::Core::Type::IsPrimetive(fieldInfo.typeId))
			{
				memcpy_s(fieldData, fieldType->size, fieldValue->value, fieldType->size);
			}
			else
			{
				if (fieldType->module == nullptr)
				{
					if (auto arrayType = dynamic_cast<const Varlet::Core::IArrayType*>(fieldType))
						RestoreArray(fieldData, fieldValue, arrayType);
				}
				else
				{
					RestoreByType(fieldData, fieldType);
				}
			}
		}
	}

	void BinaryDeserializer::RestorePointer(void* pointer, const Type* pointerType, void* value, uint32_t valueSize)
	{
		void** pointerPtr = (void**)pointer;

		if (pointerType->IsA(ISharedSerializable))
		{
			if (_sharedResources == nullptr)
				return;
			
			if (valueSize == sizeof(uint32_t) && *(uint32_t*)value == 0)
				return;

			int8_t* begin = std::bit_cast<int8_t*>(value);
			std::vector<int8_t> sharedId(begin, begin + valueSize);
			
			const auto it = std::find_if((*_sharedResources).begin(), (*_sharedResources).end(), [sharedId](SharedResourceInfo& resource)
				{
					return sharedId.size() == resource.id.size() && std::equal(sharedId.begin(), sharedId.end(), resource.id.begin());
				});

			if (it == (*_sharedResources).end())
				_sharedResources->push_back({ std::move(sharedId), { { pointerType, pointerPtr } } });
			else
				(*it).dependencies.push_back({ pointerType, pointerPtr });
		}
		else
		{
			const uint32_t instanceId = *std::bit_cast<uint32_t*>(value);
			_references[instanceId].push_back(pointerPtr);
		}
	}

	void BinaryDeserializer::RestoreArray(int8_t* value, const DeserializedFieldInfo* fieldInfo, const IArrayType* arrayType)
	{
		ByteReader reader(fieldInfo->value, fieldInfo->regionSize);

		const uint32_t size = reader.Read<uint32_t>();

		arrayType->Resize(value, size);
		int8_t* arrayData = arrayType->GetData(value);

		const bool isElementPointer = arrayType->tFlags.Has(FieldMetaFlags::Pointer);
		const auto elementType = Type::GetType(arrayType->tId);

		const uint32_t elementSize = isElementPointer
			? sizeof(size_t)
			: elementType->size;

		for (int32_t i = 0; i < size; i++)
		{
			int8_t* elementValue = arrayData + elementSize * i;

			if (isElementPointer)
			{
				const uint32_t dataSize = reader.Read<uint32_t>();
				void* pointerValue = reader.Read(dataSize);
				RestorePointer(elementValue, elementType, pointerValue, dataSize);
			}
			else
			{
				RestoreByType((int8_t*)reader.Read(), elementType);
			}
		}
	}

	void BinaryDeserializer::ResolveDependencies()
	{
		for (const auto& instsnceData : _instances)
		{
			if (_references.contains(instsnceData.first) == false)
				continue;

			for (auto referenceData : _references[instsnceData.first])
				*referenceData = instsnceData.second;
		}
	}

	DeserializedFieldInfo* BinaryDeserializer::GetValue(const FieldInfo& fieldInfo)
	{
		const uint32_t nameHasded = Type::Hash(fieldInfo.name.c_str());

		auto it = std::find_if(_currentInstanceValues.begin(), _currentInstanceValues.end(), [this, nameHasded, fieldInfo](const DeserializedFieldInfo& info)
			{
				return info.regionLevel == _currentRegionLevel
					&& info.nameHashed == nameHasded
					&& info.typeId == fieldInfo.typeId
					&& info.flags == fieldInfo.flags;
			});

		return it != _currentInstanceValues.end()
			? &(*it)
			: nullptr;
	}
}
