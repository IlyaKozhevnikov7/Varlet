#pragma once

#include "VarletCore.h"

//namespace Varlet
//{
//	template<typename ReturnValue, typename... Args>
//	struct ObserverBase
//	{
//	protected:
//
//		ObserverBase() = default;
//
//	public:
//
//		virtual ~ObserverBase() = default;
//
//		virtual ReturnValue Invoke(Args... args) const = 0;
//
//		virtual ReturnValue operator()(Args... args) = 0;
//	};
//
//	template<typename T, typename ReturnValue, typename... Args>
//	struct Observer final : ObserverBase<ReturnValue, Args...>
//	{
//		using Signature = ReturnValue(T::*)(Args...);
//
//	private:
//
//		T* _object;
//		Signature _method;
//
//	public:
//
//		Observer(T* object, Signature method)
//		{
//			_object = object;
//			_method = method;
//		}
//
//		~Observer() override = default;
//
//		ReturnValue Invoke(Args... args) const override
//		{
//			return (_object->*_method)(args...);
//		}
//	};
//}
//
//template<typename ReturnValue, typename... Args>
//struct Delegate final
//{
//	using Signature = ReturnValue(*)(Args...);
//
//private:
//
//	Varlet::ObserverBase<Signature> _observer;
//
//public:
//
//	template<typename T>
//	void Bind(T* object, ReturnValue(T::* method)(Args...))
//	{
//		_observer = Varlet::Observer<T, Signature>(object, method);
//	}
//
//	ReturnValue Invoke(Args... args) const
//	{
//		return _observer->Invoke(std::forward<Args>(args)...);
//	}
//};
//
//template<typename... Args>
//using Action = Delegate<void, Args...>;
//
//template<typename... Args>
//struct Event final
//{
//	using Signature = void(Args...);
//
//private:
//
//	std::vector<Varlet::ObserverBase<void(Args...)>> _observers;
//
//public:
//
//	template<typename T>
//	void Bind(T* object, void(T::* method)(Args...))
//	{
//		_observers.push_back(Varlet::Observer<T, Signature>(object, method));
//	}
//
//	void Invoke(Args... args) const
//	{
//		for (const auto observer : _observers)
//			observer->Invoke(std::forward<Args>(args)...);
//	}
//};
