#include "Serialization/Formatter.h"
#include "Reflection/StringType.h"
#include "Serialization/SerializationUtils.h"

namespace Varlet::Core
{
	void BinaryFormatter::SerializeAll(SerializeInfo& info, SerializationContext& context)
	{
		while (context.MoveNext())
		{
			SerializeDependence(info, context);

			if (context.flags.NoHas(SerializeFlagBit::WithDependencies))
				return;
		}
	}

	void BinaryFormatter::SerializeDependence(SerializeInfo& info, SerializationContext& context)
	{
		info.AddData(context.GetCurrentDependence().type->id);
		info.AddData(context.GetCurrentDependence().objectId);

		SerializeValue(info, context);
	}

	void BinaryFormatter::SerializeValue(SerializeInfo& info, SerializationContext& context)
	{
		int8_t* value = context.currentProcessedData;
		const Type* type = context.currentProcessedType;

		if (type->IsA(ICustomSerializable))
		{
			auto customSerializable = CastMemory<ICustomSerializable>(type, value);
			customSerializable->Serialize(info);
			return;
		}

		if (IsPrimetiveType(type->id))
		{
			info.AddData(value, type->size);
			return;
		}

		for (auto& field : type->fieldInfos)
		{
			if (field.flags.Has(FieldMetaFlags::Static))
				continue;

			context.currentProcessedData = value + field.offset;
			context.currentProcessedType = Type::GetType(field.typeId);
			context.currentProcessedDataFlags = field.flags;

			if (context.flags.Has(SerializeFlagBit::WithFieldMeta))
			{
				uint32_t hash = HashName(field.name.c_str());
				info.AddData(std::bit_cast<int8_t*>(&hash), sizeof(uint32_t));
			}

			if (context.flags.Has(SerializeFlagBit::WithDependencies) && context.currentProcessedDataFlags.Has(FieldMetaFlags::Pointer))
			{
				SerializePointer(info, context);
			}
			else if (IsPrimetiveType(field.typeId))
			{
				info.AddData(context.currentProcessedData, field.size);
			}
			else if (IsString(field.typeId))
			{
				const auto stringType = std::bit_cast<StringType*>(context.currentProcessedType);
				const uint32_t size = stringType->GetSize(context.currentProcessedData);
				const int8_t* data = stringType->GetData(context.currentProcessedData);

				info.AddData(std::bit_cast<int8_t*>(&size), sizeof(uint32_t));
				info.AddData(data, size);
			}
			else
			{
				if (context.currentProcessedType->module == nullptr)
				{
					assert(dynamic_cast<const TemplateType*>(context.currentProcessedType));

					if (dynamic_cast<const IArrayType*>(context.currentProcessedType))
					{
						SerializeArray(info, context);
						continue;
					}
				}

				SerializeValue(info, context);
			}
		}
	}

	void BinaryFormatter::SerializePointer(SerializeInfo& info, SerializationContext& context)
	{
		int8_t* value = *std::bit_cast<int8_t**>(context.currentProcessedData);

		if (value == nullptr)
		{
			info.AddData<uint32_t>(0);
			return;
		}
		
		const auto actualType = context.currentProcessedType->GetActualType(value);

		if (context.currentProcessedType->IsA(ISharedSerializable))
		{
			// TODO: Problem - context.currentProcessedType (POINTER TYPE (NO OBJECT)) MUST be based of ISharedSerializable for register obj as shared

			auto sharedSerializable = CastMemory<ISharedSerializable>(actualType, value);
			assert(sharedSerializable);

			info.AddSharedId(sharedSerializable->GetSharedId());
			return;
		}
		
		uint32_t dependenceId = context.GetDependenceId(value, actualType);
		info.AddData(dependenceId);
	}

	void BinaryFormatter::SerializeArray(SerializeInfo& info, SerializationContext& context)
	{
		auto arrayType = dynamic_cast<const IArrayType*>(context.currentProcessedType);
		assert(arrayType != nullptr);

		const bool isPointer = arrayType->tFlags.Has(FieldMetaFlags::Pointer);

		if (isPointer && context.flags.NoHas(SerializeFlagBit::WithDependencies))
		{
			// TODO: warning log

			info.AddData(0, sizeof(uint32_t));
			return;
		}
		
		context.currentProcessedType = Type::GetType(arrayType->tId);

		int8_t* arrayData = arrayType->GetData(context.currentProcessedData);
		const uint32_t size = arrayType->Size(context.currentProcessedData);
		uint32_t tSize = isPointer
			? sizeof(size_t)
			: context.currentProcessedType->size;

		info.AddData(std::bit_cast<int8_t*>(&size), sizeof(uint32_t));

		for (uint32_t i = 0; i < size; i++)
		{
			context.currentProcessedData = arrayData + (tSize * i);

			if (isPointer)
			{
				SerializePointer(info, context);
			}
			else
			{
				SerializeValue(info, context);
			}
		}
	}

	void BinaryFormatter::DeserializeValue(DeserializeInfo& info)
	{
		if (info.currentProcessedType->IsA(ICustomSerializable))
		{
			auto customSerializable = CastMemory<ICustomSerializable>(info.currentProcessedType, info.currentProcessedData);
			customSerializable->Deserialize(info);
			return;
		}

		if (IsPrimetiveType(info.currentProcessedType->id))
		{
			info.ReadTo(info.currentProcessedData, info.currentProcessedType->size);
			return;
		}

		auto processedType = info.currentProcessedType;
		int8_t* instanceTop = info.currentProcessedData;

		for (auto& field : processedType->fieldInfos)
		{
			if (field.flags.Has(FieldMetaFlags::Static))
				continue;

			info.currentProcessedData = instanceTop + field.offset;
			info.currentProcessedType = Type::GetType(field.typeId);

			if ((field.flags.Has(FieldMetaFlags::Pointer)))
			{
				DeserializePointer(info);
			}
			else if (auto customSerializable = CastMemory<ICustomSerializable>(info.currentProcessedType, info.currentProcessedData))
			{
				customSerializable->Deserialize(info);
			}
			else if (IsPrimetiveType(field.typeId))
			{
				info.ReadTo(info.currentProcessedData, field.size);
			}
			else
			{
				if (info.currentProcessedType->module == nullptr)
				{
					assert(dynamic_cast<const TemplateType*>(info.currentProcessedType));

					if (dynamic_cast<const IArrayType*>(info.currentProcessedType))
					{
						DeserializeArray(info);
						continue;
					}
				}

				DeserializeValue(info);
			}
		}
	}

	void BinaryFormatter::DeserializePointer(DeserializeInfo& info)
	{
		if (info.currentProcessedType->IsA(ISharedSerializable))
		{
			uint32_t size = info.Read<uint32_t>();

			if (size > 0)
			{
				std::vector<int8_t> sharedObjId;
				sharedObjId.resize(size);

				info.ReadTo(&sharedObjId[0], size);
				info.AddSharedData(info.currentProcessedType, sharedObjId, info.currentProcessedData);
			}
		}
		else
		{
			info.ReadPointer(info.currentProcessedData);
		}
	}

	bool BinaryFormatter::IsPrimetiveType(const uint32_t& id)
	{
		static std::vector<uint32_t> primitiveTypeIds =
		{
			Type::GetType(NAME_OF(bool))->id,
			Type::GetType(NAME_OF(int8_t))->id,
			Type::GetType(NAME_OF(uint8_t))->id,
			Type::GetType(NAME_OF(int16_t))->id,
			Type::GetType(NAME_OF(uint16_t))->id,
			Type::GetType(NAME_OF(int32_t))->id,
			Type::GetType(NAME_OF(uint32_t))->id,
			Type::GetType(NAME_OF(int64_t))->id,
			Type::GetType(NAME_OF(uint64_t))->id,
			Type::GetType(NAME_OF(float))->id,
			Type::GetType(NAME_OF(double))->id,
			Type::GetType(NAME_OF(glm::bvec2))->id,
			Type::GetType(NAME_OF(glm::bvec3))->id,
			Type::GetType(NAME_OF(glm::bvec4))->id,
			Type::GetType(NAME_OF(glm::ivec2))->id,
			Type::GetType(NAME_OF(glm::ivec3))->id,
			Type::GetType(NAME_OF(glm::ivec4))->id,
			Type::GetType(NAME_OF(glm::uvec2))->id,
			Type::GetType(NAME_OF(glm::uvec3))->id,
			Type::GetType(NAME_OF(glm::uvec4))->id,
			Type::GetType(NAME_OF(glm::vec2))->id,
			Type::GetType(NAME_OF(glm::vec3))->id,
			Type::GetType(NAME_OF(glm::vec4))->id,
			Type::GetType(NAME_OF(glm::dvec2))->id,
			Type::GetType(NAME_OF(glm::dvec3))->id,
			Type::GetType(NAME_OF(glm::dvec4))->id,
			Type::GetType(NAME_OF(glm::mat2))->id,
			Type::GetType(NAME_OF(glm::mat3))->id,
			Type::GetType(NAME_OF(glm::mat4))->id,
		};

		return std::find(primitiveTypeIds.begin(), primitiveTypeIds.end(), id) != primitiveTypeIds.end();
	}

	bool BinaryFormatter::IsString(const uint32_t& id)
	{
		static uint32_t stringTypeIds = Type::GetType(NAME_OF(std::string))->id;
		return id == stringTypeIds;
	}

	int32_t BinaryFormatter::HashName(const char* name)
	{
		uint32_t hash = 5381;
		int32_t c;

		while (c = *name++)
			hash = ((hash << 5) + hash) + c;

		return hash;
	}
}