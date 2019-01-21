#pragma once

/**
 * This delegate implementation does not support functions
 * taking a rvalue reference as parameter.
 */

#define MakeDelegate(_funcptr, ...) (Vague::GetDelegateFactory(_funcptr).Make<_funcptr>(__VA_ARGS__))

namespace Vague
{
    template <class FunctionSignature>
    class Delegate;

    template <class ReturnType, class... Param>
    class Delegate<ReturnType (Param...)>
    {
    public:

        using WrapperPtr = ReturnType (*)(const void*, Param&...);

        Delegate()
            : m_ftPtr(nullptr), m_callee(nullptr)
        {
        }

        Delegate(WrapperPtr _ftPtr, const void *_callee)
            : m_ftPtr(_ftPtr), m_callee(_callee)
        {
        }

        bool operator==(const Delegate &_rhs) const
        {
            return (m_ftPtr == _rhs.m_ftPtr) && (m_callee == _rhs.m_callee);
        }

        bool operator!=(const Delegate &_rhs) const
        {
            return (m_ftPtr != _rhs.m_ftPtr) && (m_callee != _rhs.m_callee);
        }

        /**
         * This template is there to force type deduction,
         * allowing the use of 'universal reference'
         */
        template <class... ParamDeduction>
        ReturnType operator()(ParamDeduction&&... _p) const
        {
            return m_ftPtr(m_callee, _p...);
        }

        template <class T, ReturnType (T::*Func)(Param...)>
        static ReturnType MethodWrapper(const void *_callee, Param&... _p)
        {
            T *callee = const_cast<T*>(static_cast<const T*>(_callee));
            return (callee->*Func)(_p...);
        }

        template <class T, ReturnType (T::*Func)(Param...) const>
        static ReturnType ConstMethodWrapper(const void *_callee, Param&... _p)
        {
            const T *callee = static_cast<const T*>(_callee);
            return (callee->*Func)(_p...);
        }

        template <ReturnType (*Func)(Param...)>
        static ReturnType FunctionWrapper(const void *_callee, Param&... _p)
        {
            return (*Func)(_p...);
        }

    private:
        WrapperPtr m_ftPtr;
        const void *m_callee;
    };

    template <class T, class ReturnType, class... Param>
    class MethodDelegateFactory
    {
    public:
        template <ReturnType (T::*Func)(Param...)>
        static Delegate<ReturnType (Param...)> Make(T *_callee)
        {
            return Delegate<ReturnType (Param...)>(
                    &Delegate<ReturnType (Param...)>::MethodWrapper<T, Func>,
                    _callee);
        }
    };

    template <class T, class ReturnType, class... Param>
    class ConstMethodDelegateFactory
    {
    public:
        template <ReturnType (T::*Func)(Param...) const>
        static Delegate<ReturnType (Param...)> Make(T *_callee)
        {
            return Delegate<ReturnType (Param...)>(
                    &Delegate<ReturnType (Param...)>::ConstMethodWrapper<T, Func>,
                    _callee);
        }
    };

    template <class ReturnType, class... Param>
    class FunctionDelegateFactory
    {
    public:
        template <ReturnType (*Func)(Param...)>
        static Delegate<ReturnType (Param...)> Make()
        {
            return Delegate<ReturnType (Param...)>(
                    &Delegate<ReturnType (Param...)>::FunctionWrapper<Func>,
                    nullptr);
        }
    };

    template<class T, class ReturnType, class... Param>
    MethodDelegateFactory<T, ReturnType, Param...> GetDelegateFactory(ReturnType (T::*)(Param...))
    {
        return MethodDelegateFactory<T, ReturnType, Param...>();
    }

    template<class T, class ReturnType, class... Param>
    ConstMethodDelegateFactory<T, ReturnType, Param...> GetDelegateFactory(ReturnType (T::*)(Param...) const)
    {
        return ConstMethodDelegateFactory<T, ReturnType, Param...>();
    }

    template<class ReturnType, class... Param>
    FunctionDelegateFactory<ReturnType, Param...> GetDelegateFactory(ReturnType (*)(Param...))
    {
        return FunctionDelegateFactory<ReturnType, Param...>();
    }
}
