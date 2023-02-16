#include "Reflection.h"

namespace Varlet::Core
{
	FieldInfo::FieldInfo(const char* name, const char* typeName, const size_t& offset, const size_t& size) :
		name(name),
		typeId(Type::Hash(typeName)),
		offset(offset),
		size(size)
	{
	}

	std::unordered_map<const Module*, std::vector<const Type*>> Type::_types;

	void Type::Register(const Module* module, const Type* type)
	{
		for (auto& moduleTypes : _types)
			for (auto& element : moduleTypes.second)
				if (element == type || element->id == type->id)
					return;

		_types[module].push_back(type);
	}

	void Type::UnregisterModuleTypes(const Module* module)
	{
		if (_types.contains(module))
			_types.erase(module);
	}

	const Type* Type::GetType(const char* name)
	{
		const uint32_t id = Hash(name);

		for (auto& moduleTypes : _types)
			for (auto& element : moduleTypes.second)
				if (element->id == id)
					return element;

		return nullptr;
	}

	std::vector<const Type*>& Type::GetTypes(const Module* module)
	{
		return _types[module];
	}

	std::vector<const Type*> Type::GetAllTypes()
	{
		std::vector<const Type*> types;

		for (auto& moduleTypes : _types)
			types.insert(types.begin(), moduleTypes.second.begin(), moduleTypes.second.end());

		return types;
	}

	Type::Type(const char* name, const size_t& size, std::initializer_list<const char*> parentsNames, std::initializer_list<FieldInfo> fieldInfos, CreateFunction createFunction) :
		name(name),
		id(Hash(name)),
		size(size),
		parentsNames(parentsNames),
		fieldInfos(fieldInfos),
		_createFunction(createFunction)
	{
	}

	Type& Type::operator+(const Type* other)
	{
		parentsNames.insert(parentsNames.begin(), other->parentsNames.begin(), other->parentsNames.end());
		fieldInfos.insert(fieldInfos.begin(), other->fieldInfos.begin(), other->fieldInfos.end());

		return *this;
	}

	void* Type::CreateInstance() const
	{
		return _createFunction();
	}

	bool Type::IsA(const char* typeName) const
	{
		const uint32_t id = Hash(typeName);

		return IsA(id);
	}

	bool Type::IsA(const uint32_t& typeId) const
	{
		if (id == typeId)
			return true;

		for (auto parent : parentsNames)
		{
			const uint32_t parentId = Hash(parent);

			if (parentId == typeId)
				return true;
		}

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