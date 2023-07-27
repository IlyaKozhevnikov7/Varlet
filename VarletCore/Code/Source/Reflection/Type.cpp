#include "Reflection/TemplateType.h"
#include "Reflection/Enum.h"

namespace Varlet::Core
{
	FieldInfo::FieldInfo(const char* name, const char* typeName, size_t offset, size_t size, BitMask<FieldMetaFlags> flags) :
		name(name),
		typeId(Type::Hash(typeName)),
		offset(offset),
		size(size),
		flags(flags)
	{
	}

	std::vector<const Type*> Type::_types;

	void Type::Register(const Module* module, const Type* type)
	{
		assert(type);

		auto mutableType = const_cast<Type*>(type);

		if (auto templateType = dynamic_cast<TemplateType*>(mutableType))
		{
			auto sameTemplate = dynamic_cast<TemplateType*>(const_cast<Type*>(GetType(type->name.c_str())));
			if (sameTemplate == nullptr)
			{
				templateType->used.push_back(module);
				_types.push_back(templateType);
				return;
			}

			sameTemplate->used.push_back(module);
		}
		else
		{
			// TODO: check is type registered

			mutableType->module = module;
			_types.push_back(type);
		}

		if (type->flags.Has(TypeMetaFlags::Enum))
		{

		}
	}

	void Type::UnregisterModuleTypes(const Module* module)
	{
		for (auto it = _types.begin(); it != _types.end(); )
		{
			auto type = *it;

			if (type->module == module)
			{
				it = _types.erase(it);
			}
			else if (type->flags.Has(TypeMetaFlags::GeneratedType))
			{
				auto templateType = dynamic_cast<TemplateType*>(const_cast<Type*>(type));
				assert(templateType);

				for (auto& useModule : templateType->used)
					if (module == useModule)
						templateType->used.erase(std::remove(templateType->used.begin(), templateType->used.end(), module), templateType->used.end());

				if (templateType->used.size() == 0)
					it = _types.erase(it);
				else
					++it;
			}
			else
			{
				++it;
			}
		}
	}

	const Type* Type::GetType(const char* name)
	{
		const uint32_t id = Hash(name);
		return GetType(id);
	}

	const Type* Type::GetType(const uint32_t id)
	{
		for (auto& type : _types)
			if (type->id == id)
				return type;

		return nullptr;
	}

	uint32_t Type::GetTemplateId(const char* name)
	{
		const uint32_t hash = Hash(name);

		for (auto& type : _types)
			if (type->flags.Has(TypeMetaFlags::GeneratedType))
			{
				auto generatedType = std::bit_cast<TemplateType*>(type);
				assert(generatedType);

				if (generatedType->templateId == hash)
					return hash;
			}

		return 0;
	}

	std::vector<const Type*> Type::GetTypes(const Module* module)
	{
		std::vector<const Type*> types;

		for (auto& type : _types)
			if (type->module == module)
				types.push_back(type);

		return types;
	}

	const std::vector<const Type*>& Type::GetAllTypes()
	{
		return _types;
	}

	Type::Type(const char* name, const size_t& size, std::initializer_list<ParentInfo> parentInfos, std::initializer_list<FieldInfo> fieldInfos, GetActualTypeFunction getActualTypeFunction, CreateFunction createFunction, BitMask<TypeMetaFlags> flags) :
		name(name),
		id(Hash(name)),
		size(size),
		parentInfos(parentInfos),
		fieldInfos(fieldInfos),
		_getActualTypeFunction(getActualTypeFunction),
		_createFunction(createFunction),
		flags(flags)
	{
	}

	Type::Type(const char* name, const size_t& size, std::initializer_list<EnumValue> values) :
		name(name),
		id(Hash(name)),
		size(size),
		parentInfos({}),
		fieldInfos({}),
		_getActualTypeFunction(nullptr),
		_createFunction(nullptr),
		flags(TypeMetaFlags::Enum)
	{
		Enum::_enumValues[this] = values;
	}

	Type::~Type()
	{
		//if (flags.Has(TypeMetaFlags::Enum))
		//	Enum::_enumValues.erase(this);
	}

	Type& Type::operator+(const Type* other)
	{
		parentInfos.insert(parentInfos.begin(), other->parentInfos.begin(), other->parentInfos.end());
		fieldInfos.insert(fieldInfos.begin(), other->fieldInfos.begin(), other->fieldInfos.end());

		return *this;
	}

	void* Type::CreateInstance() const
	{
		return _createFunction();
	}

	const Type* Type::GetActualType(void* value) const
	{
		return _getActualTypeFunction(value);
	}

	const FieldInfo* Type::GetFieldInfo(const char* name) const
	{
		for (auto& info : fieldInfos)
			if (info.name == name)
				return &info;

		return nullptr;
	}

	void* Type::GetFieldData(void* instance, const char* name) const
	{
		for (auto& info : fieldInfos)
			if (info.name == name)
				return std::bit_cast<int8_t*>(instance) + info.offset;

		return nullptr;
	}

	bool Type::CheckIsA(const Type* type) const
	{
		return CheckIsA(type->id);
	}

	bool Type::CheckIsA(const uint32_t& typeId) const
	{
		if (id == typeId)
			return true;

		for (auto info : parentInfos)
			if (info.type->CheckIsA(typeId))
				return true;

		return false;
	}

	uint32_t Type::Hash(const char* name)
	{
		uint32_t hash = 5381;
		int32_t c;

		while (c = *name++)
			hash = ((hash << 5) + hash) + c;

		return hash;
	}
}

void* CastMemory(const Varlet::Core::Type* fromType, const Varlet::Core::Type* toType, int8_t* top)
{
	// TODO: check how to work if try cast null
	assert(top);

	if (fromType->CheckIsA(toType) == false)
		return nullptr;

	for (auto& info : fromType->parentInfos)
		if (info.type == toType)
			return top + info.vfPtrOffset;

	return top;
}
