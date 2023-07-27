#include "Reflection/TemplateType.h"

namespace Varlet::Core
{
	TemaplateParameter::TemaplateParameter(const char* name, BitMask<TypeMetaFlags> flags) :
		id(Type::Hash(name)),
		flags(flags)
	{
	}

	TemplateType::TemplateType(const char* name, const char* templateName, size_t size, std::vector<TemaplateParameter>&& parameterList, std::initializer_list<ParentInfo> parentInfos, std::initializer_list<FieldInfo> fieldInfos, Type::GetActualTypeFunction getActualTypeFunction, Type::CreateFunction createFunction, BitMask<TypeMetaFlags> flags) :
		templateId(Type::Hash(templateName)),
		parameterList(parameterList),
		Type(name, size, parentInfos, fieldInfos, getActualTypeFunction, createFunction, flags | TypeMetaFlags::GeneratedType)
	{
	}

	TemplateType& TemplateType::operator+(const Type* other)
	{
		this->Type::operator+(other);
		return *this;
	}

	IArrayType::IArrayType(const char* name, size_t size, const char* tName, const BitMask<FieldMetaFlags>::MaskType& flags) :
		tId(Type::Hash(tName)),
		tFlags(flags),
		TemplateType(name, "std::vector", size, {}, {}, {}, nullptr, nullptr)
	{
	}
}
