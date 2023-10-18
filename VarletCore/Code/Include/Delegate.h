#pragma once

#include <vector>
#include <type_traits>

template<typename... Args>
struct Event;

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
	struct Observer final : public ObserverBase<ReturnValue, Args...>
	{
		template<typename... Args>
		friend struct Event;

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
struct Function final : public Varlet::ObserverBase<ReturnValue, Args...>
{
	template<typename... Args>
	friend struct Event;

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
		if (_observer != nullptr)
			return _observer->Invoke(args...);

		if constexpr (std::is_pointer_v<ReturnValue>)
			return nullptr;
		else
			return {};
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

	template<typename T>
	void UnBind(T* object, void(T::*method)(Args...))
	{
		const auto it = std::find_if(_observers.begin(), _observers.end(), [object, method](const Varlet::ObserverBase<void, Args...>* observerBase)
			{
				const auto observer = dynamic_cast<const Varlet::Observer<T, void, Args...>*>(observerBase);
				return observer && observer->_object == object && observer->_method == method;
			});

		if (it != _observers.end())
		{
			delete (*it);
			_observers.erase(it);
		}
	}

	void UnBind(void(*method)(Args...))
	{
		const auto it = std::find_if(_observers.begin(), _observers.end(), [method](const Varlet::ObserverBase<void, Args...>* observerBase)
			{
				const auto function = dynamic_cast<const Function<void, Args...>*>(observerBase);
				return function && function->_method == method;
			});

		if (it != _observers.end())
			_observers.erase(it);
	}

	void Invoke(Args... args) const
	{
		for (const auto observer : _observers)
			observer->Invoke(args...);
	}
};
