#pragma once

#include <type_traits>

template<auto Bit>
constexpr auto BIT = 1 << (Bit - 1);

namespace Varlet
{
	using Descriptor = int8_t;
}

#define NAME_OF(Value) #Value
#define PAD(Bites) int8_t __pad[Bites];

#if defined(VARLET_DEVELOPMENT)
	#define VARLET_CONFIGURATION_NAME "Development"
#else
	#define VARLET_CONFIGURATION_NAME "Release"
#endif

#define WIDE(x) L##x

namespace Varlet::Core
{
	template<size_t Size>
	struct BitField;

	template<>
	struct BitField<1>
	{
		using Type = int8_t;
	};

	template<>
	struct BitField<2>
	{
		using Type = int16_t;
	};

	template<>
	struct BitField<4>
	{
		using Type = int32_t;
	};

	template<>
	struct BitField<8>
	{
		using Type = int64_t;
	};
}

template<typename TEnum, size_t TSize = sizeof(TEnum)>
struct BitMask final
{
	static_assert(std::is_enum_v<TEnum> || std::is_integral_v<TEnum>);

	using ThisType = BitMask<TEnum, TSize>;
	using MaskType = Varlet::Core::BitField<TSize>::Type;

private:

	MaskType _mask = 0;

public:
	
	BitMask() = default;
	BitMask(const ThisType&) = default;

	BitMask(MaskType mask) :
		_mask(mask)
	{
	}

	BitMask(ThisType&& other)
	{
		_mask = other._mask;
		other._mask = 0;
	}

	ThisType& operator=(const ThisType& other) noexcept
	{
		_mask = other._mask;
		return *this;
	}

	MaskType Get() const
	{
		return _mask;
	}
	
	bool operator==(const ThisType& other) const noexcept
	{
		return _mask == other._mask;
	}

	bool operator!=(const ThisType& other) const noexcept
	{
		return _mask != other._mask;
	}

	ThisType operator&(MaskType mask) const
	{
		return _mask & mask;
	}
	
	ThisType operator|(MaskType mask) const
	{
		return _mask | mask;
	}

	void operator|=(MaskType mask)
	{
		_mask |= mask;
	}
	
	ThisType operator^(MaskType mask) const
	{
		return _mask ^ mask;
	}
	
	ThisType operator~()
	{
		_mask = ~_mask;
		return _mask;
	}
	
	bool Has(const ThisType& other) const
	{
		return (_mask & other._mask) == other._mask;
	}

	bool NoHas(const ThisType& other) const
	{
		return (_mask & other._mask) != other._mask;
	}
};
