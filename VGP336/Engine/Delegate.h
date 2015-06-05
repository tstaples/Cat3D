#ifndef INCLUDED_ENGINE_DELEGATE_H
#define INCLUDED_ENGINE_DELEGATE_H

//====================================================================================================
// Filename:	Delegate.h
// Created by:	Peter Chan
// Description:	Simple delegate implementation based on the following link:
//				http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates
// TODO:		For a truly generic version, refer to (requires C++11 Variadic Template support):
//				http://blog.coldflake.com/posts/C++-delegates-on-steroids/
//====================================================================================================

//====================================================================================================
// Macros
//====================================================================================================

#define DELEGATE(PARAM_CALLBACK, PARAM_OWNER)\
    (MakeDelegate(PARAM_CALLBACK).Bind<PARAM_CALLBACK>(PARAM_OWNER))

//====================================================================================================
// Class Declarations
//====================================================================================================

template <class ReturnType, typename... Params>
class Delegate
{
	typedef ReturnType (*InvokeType)(void*, Params...);
public:
    Delegate()
		: mInstance(nullptr)
        , mInvoke(nullptr)
    {}
	Delegate(void* callee, InvokeType callback)
		: mInstance(callee)
		, mInvoke(callback)
	{}

	ReturnType operator()(Params... p1) const
	{
		return (*mInvoke)(mInstance, p1...);
	}

    Delegate<ReturnType, Params...>& operator=(const Delegate<ReturnType, Params...>& rhs)
    {
        mInstance = rhs.mInstance;
        mInvoke = rhs.mInvoke;
        return *this;
    }

	bool Empty() const
	{
		return mInstance == nullptr || mInvoke == nullptr;
	}

	void Clear()
	{
		mInstance = nullptr;
		mInvoke = nullptr;
	}
	
	template <class T, ReturnType (T::*MethodType)(Params...)>
	static Delegate Make(T* instance)
	{
		Delegate d(instance, &Invoke<T, MethodType>);
		return d;
	}

private:
	void* mInstance;
	InvokeType mInvoke;

	template <class T, ReturnType (T::*MethodType)(Params...)>
	static ReturnType Invoke(void* instance, Params... p1)
	{
		T* ptr = static_cast<T*>(instance);
		return (ptr->*MethodType)(p1...);
	}
};

template <typename T, class ReturnType, typename... Params>
struct DelegateMaker
{
    template <ReturnType (T::*MethodType)(Params...)>
    static ReturnType Invoker(void* o, Params... p)
    {
        return (static_cast<T*>(o)->*MethodType)(p...);
    }

    template <ReturnType (T::*MethodType)(Params...)>
    inline static Delegate<ReturnType, Params...> Bind(T* o)
    {
        return Delegate<ReturnType, Params...>(o, &DelegateMaker::Invoker<MethodType>);
    }
};

template <typename T, typename ReturnType, typename... Params>
DelegateMaker<T, ReturnType, Params...>
MakeDelegate(ReturnType (T::*)(Params...))
{
    return DelegateMaker<T, ReturnType, Params...>();
}

#endif // #ifndef INCLUDED_ENGINE_DELEGATE_H