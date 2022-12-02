#pragma once

#include <type_traits>

template<typename ForType, typename IdType>
class IIndexed
{
	static_assert(std::_Is_any_of_v<IdType, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t, size_t>);

private:

	static IdType _idCounter;
	IdType _id;

public:

	IIndexed()
	{
		_id = _idCounter;
		++_idCounter;
	}

	virtual ~IIndexed() = default;

	IdType GetId() const
	{
		return _id;
	}
};

template<typename ForType, typename IdType>
IdType IIndexed<ForType, IdType>::_idCounter = std::numeric_limits<IdType>::min();