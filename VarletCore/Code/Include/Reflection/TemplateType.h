#pragma once

#include "Reflection/Type.h"

namespace Varlet::Core
{
	struct CORE_API TemaplateParameter final
	{
		uint32_t id;
		BitMask<TypeMetaFlags> flags;

		TemaplateParameter& operator=(const TemaplateParameter&) = default;

		TemaplateParameter(const char* name, BitMask<TypeMetaFlags> flags);
	};

	struct CORE_API TemplateType : public Type
	{
	public:

		/*
		* Hashed template name
		*
		* template<typename T>
		* string Template {}
		*
		* using: Template<int32_t>, Template<void*>
		*
		* Template name is "Template"
		*/
		uint32_t templateId = 0;

		std::vector<TemaplateParameter> parameterList;
		std::vector<const Module*> used;

	public:

		TemplateType(const char* name, const char* templateName, size_t size, std::vector<TemaplateParameter>&& parameterList, std::initializer_list<ParentInfo> parentInfos, std::initializer_list<FieldInfo> fieldInfos, Type::GetActualTypeFunction getActualTypeFunction, Type::CreateFunction createFunction, BitMask<TypeMetaFlags> flags = 0);

		virtual ~TemplateType() override = default;

		TemplateType& operator+(const Type* other);
	};

	struct CORE_API IArrayType : public TemplateType
	{
	public:

		uint32_t tId;
		BitMask<FieldMetaFlags> tFlags;

	public:

		IArrayType(const char* name, size_t size, const char* tName, const BitMask<FieldMetaFlags>::MaskType& flags);

		virtual ~IArrayType() = default;

		virtual void Add(void* instance) const = 0;

		virtual void Delete(void* instance, size_t index) const = 0;

		virtual void Resize(void* instance, size_t newSize) const = 0;

		virtual size_t Size(void* instance) const = 0;

		virtual int8_t* GetData(void* instance) const = 0;	
	};

	template<typename T>
	struct TemplateArrayType final : public IArrayType
	{
		TemplateArrayType(const char* name, const char* tName, const uint8_t flags) :
			IArrayType(name, sizeof(std::vector<T>), tName, flags)
		{
		}

		~TemplateArrayType() override = default;

		void Add(void* instance) const override
		{
			auto vector = std::bit_cast<std::vector<T>*>(instance);
			
			if constexpr (std::is_pointer_v<T>)
			{
				vector->push_back(nullptr); //newElement = nullptr;
			}
			else
			{
				T newElement;
				memset(&newElement, 0, sizeof(T));
				vector->push_back(newElement);
			}
		}

		void Delete(void* instance, size_t index) const override
		{
			auto vector = std::bit_cast<std::vector<T>*>(instance);
			vector->erase(vector->begin() + index);
		}

		void Resize(void* instance, size_t newSize) const override
		{
			auto vector = std::bit_cast<std::vector<T>*>(instance);
			vector->resize(newSize);
		}

		// TODO: rename add get
		size_t Size(void* instance) const override
		{
			return std::bit_cast<std::vector<T>*>(instance)->size();
		}

		int8_t* GetData(void* instance) const override
		{
			return std::bit_cast<int8_t*>(std::bit_cast<std::vector<T>*>(instance)->data());
		}
	};
}
