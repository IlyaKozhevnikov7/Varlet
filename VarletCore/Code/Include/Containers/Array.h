#pragma once

#include "BaseContainer.h"
#include <initializer_list>

template<typename T, float ReallocMul = 2.f>
struct Array final : public BaseContainer
{
private:

	constexpr static size_t defaultCapacity = 4;

	T* _data = nullptr;
	size_t _size;
	size_t _capacity;

public:

	Array(Array& other)
	{
	}

	Array(Array&& other)
	{
		_data = other._data;
		other._data = nullptr;
	}

	constexpr Array() :
		_size(0),
		_capacity(defaultCapacity)
	{
		Allocate();
	}
	
	Array(size_t capacity) :
		_size(0),
		_capacity(_size)
	{
	}

	Array(std::initializer_list<T> elements) :
		_data(elements.begin()),
		_size(elements.size()),
		_capacity(_size)
	{
	}

	~Array()
	{
		delete _data;
	}

	size_t GetSize()
	{
		return _size;
	}

	size_t GetCapacity()
	{
		return _capacity;
	}

	void AddRange(std::initializer_list<T> elements)
	{
		Insert(elements.begin(), elements.size());
	}

	void Add(T& value)
	{

	}

	void Add(T&& value)
	{

	}

	void Remove(T& value)
	{

	}

	void Remove(T&& value)
	{

	}

	void Insert(T&& value, size_t index)
	{
		assert(index < _size);

	}

	// Find

private:

	void Insert(T* value, size_t size = 1, size_t index = _size)
	{
		const size_t newSize = _size + elements.size();

		if (newSize > _capacity)
		{
			T* newData = new T[_capacity * ReallocMul];

			if constexpr (std::is_move_assignable_v<T>)
			{
				for (size_t i = 0; i < _size; i++)
					newData = std::move(entry);
			}
			else
			{
				memcpy_s(newData, _size, _data, _size);
			}
		}
	}

	void Allocate()
	{
		_data = new T[_capacity];
	}
};