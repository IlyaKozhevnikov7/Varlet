#pragma once

#include "VarletCore.h"
#include <vector>

namespace Varlet
{
	template<typename ReturnValue, typename... Args>
	struct ObserverBase
	{
	protected:

		ObserverBase() = default;

	public:

		virtual ~ObserverBase() = default;

		virtual ReturnValue Invoke(Args... args) const = 0;
	};

	template<typename T, typename ReturnValue, typename... Args>
	struct Observer final : ObserverBase<ReturnValue, Args...>
	{
		using Signature = ReturnValue(T::*)(Args...);

	private:

		T* _object;
		Signature _method;

	public:

		Observer(T* object, Signature method)
		{
			_object = object;
			_method = method;
		}

		~Observer() override = default;

		ReturnValue Invoke(Args... args) const override
		{
			return (_object->*_method)(args...);
		}
	};
}

template<typename ReturnValue, typename... Args>
struct Function final : Varlet::ObserverBase<ReturnValue, Args...>
{
	using Signature = ReturnValue(*)(Args...);

private:

	Signature _method;

public:

	Function(Signature method)
	{
		_method = method;
	}

	~Function() override = default;

	ReturnValue Invoke(Args... args) const override
	{
		return _method(args...);
	}
};

template<typename ReturnValue, typename... Args>
struct Delegate final
{
	using Signature = ReturnValue(*)(Args...);

private:

	Varlet::ObserverBase<Signature>* _observer;

public:

	~Delegate()
	{
		delete _observer;
	}

	template<typename T>
	void Bind(T* object, ReturnValue(T::* method)(Args...))
	{
		_observer = new Varlet::Observer<T, void, Args...>(object, method);
	}

	ReturnValue Invoke(Args... args) const
	{
		return _observer->Invoke(args...);
	}
};

template<typename... Args>
using Action = Delegate<void, Args...>;

template<typename... Args>
struct Event final
{
private:

	std::vector<Varlet::ObserverBase<void, Args...>*> _observers;

public:

	template<typename T>
	void Bind(T* object, void(T::* method)(Args...))
	{
		_observers.push_back(new Varlet::Observer<T, void, Args...>(object, method));
	}

	void Bind(void(*method)(Args...))
	{
		_observers.push_back(new Function<void, Args...>(method));
	}

	void Invoke(Args... args) const
	{
		for (const auto observer : _observers)
			observer->Invoke(args...);
	}
};
