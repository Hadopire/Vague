#pragma once

#include "Core.hpp"
#include "ClassTraits.hpp"
#include "Types.hpp"

#include <type_traits>


namespace Vague
{
    #define VAGUE_STATIC_ASSERT_NOT_ARRAY(_Class)   VAGUE_STATIC_ASSERT_MSG((!std::is_array_v<_Class>), "Do not use smart pointers with arrays!")

    template <class T>
    class PtrHandler
    {
        VAGUE_STATIC_ASSERT_NOT_ARRAY(T);

        template<class U> friend class PtrHandler;

    public:

        using PtrType = std::remove_extent_t<T>;

        // Constructors
        PtrHandler()
            : m_ptr(nullptr)
        {
        }

        explicit PtrHandler(PtrType* _p)
            : m_ptr(_p)
        {
        }

        template <class U, class = typename std::enable_if_t<std::is_convertible<typename PtrHandler<U>::PtrType*, PtrType*>::value>>
        explicit PtrHandler(typename PtrHandler<U>::PtrType* _p)
            : m_ptr(static_cast<PtrType*>(_p))
        {
        }

        PtrHandler(const PtrHandler& _p)
            : m_ptr(_p.m_ptr)
        {
        }

        template <class U, class = typename std::enable_if_t<std::is_convertible<typename PtrHandler<U>::PtrType*, PtrType*>::value>>
        explicit PtrHandler(const PtrHandler<U>& _p)
            : m_ptr(static_cast<PtrType*>(_p.m_ptr))
        {
        }

        PtrHandler(PtrHandler&& _p)
            : m_ptr(_p.m_ptr)
        {
            _p.m_ptr = nullptr;
        }

        template <class U, class = typename std::enable_if_t<std::is_convertible<typename PtrHandler<U>::PtrType*, PtrType*>::value>>
        explicit PtrHandler(PtrHandler<U>&& _p)
            : m_ptr(static_cast<PtrType*>(_p.m_ptr))
        {
            _p.m_ptr = nullptr;
        }

        // Destructor
        ~PtrHandler()
        {
            m_ptr = nullptr;
        }

        PtrType*    Get() const                 { return m_ptr; }

        explicit    operator PtrType* () const { return Get(); }
        PtrType&    operator *  () const       { return *Get(); }
        PtrType*    operator -> () const       { return Get(); }

    protected:

        PtrType*    m_ptr;
    };


    // Unique pointer, delete object when destroyed

    template <class T>
    class UniquePtr
        : public PtrHandler<T>
    {
        VAGUE_NONCOPYABLE(UniquePtr);

    public:

        // Constructors
        UniquePtr()
            : PtrHandler<T>()
        {
        }

        explicit UniquePtr(PtrType* _p)
            : PtrHandler<T>(_p)
        {
        }

        template <class U>
        explicit UniquePtr(typename PtrHandler<U>::PtrType* _p)
            : PtrHandler<T>(_p)
        {
        }

        UniquePtr(UniquePtr&& _p)
            : PtrHandler<T>(std::forward<PtrHandler<T>>(_p))
        {
        }

        template <class U>
        explicit UniquePtr(UniquePtr<U>&& _p)
            : PtrHandler<T>(std::forward<PtrHandler<U>>(_p))
        {
            VAGUE_STATIC_ASSERT_MSG(std::has_virtual_destructor_v<PtrType>, "Deleting from base class without virtual destructor is UndefinedBehaviour");
        }

        // Destructor
        ~UniquePtr()
        {
            Release();
        }

        UniquePtr&  operator = (PtrType* _p)
        {
            Reset(_p);
            return *this;
        }

        UniquePtr&  operator = (UniquePtr&& _p)
        {
            Swap(_p);
            return *this;
        }

        void Reset(PtrType* _p)
        {
            if (_p != m_ptr)
            {
                Release();
                m_ptr = _p;
            }
        }

        void Release()
        {
            VAGUE_SAFE_DELETE(m_ptr);
        }

        void Swap(UniquePtr& _p)
        {
            PtrType* pTmp = _p.m_ptr;
            _p.m_ptr = m_ptr;
            m_ptr = pTmp;
        }
    };


    // Reference counter pointer, intrusive implementation

    // NOT thread-safe
    class IntrusiveRefCount
    {
        VAGUE_NONCOPYABLE(IntrusiveRefCount);

    public:

        IntrusiveRefCount()
            = default;

        // Oh no I want to remove this virtual...
        virtual ~IntrusiveRefCount()
        {
            // Not ok to delete it if refcount is not 0
            VAGUE_ASSERT(m_refCount == 0);
        }

        u32 AddRef()
        {
            return ++m_refCount;
        }

        u32 ReleaseRef()
        {
            VAGUE_ASSERT(m_refCount > 0);
            return --m_refCount;
        }

        u32 GetRefCount() const
        {
            return m_refCount;
        }

    private:

        u32 m_refCount = 0;
    };


    template <class T>
    class SharedPtr
        : public PtrHandler<T>
    {
        // How to be sure the class inherits from IntrusiveRefCount, while accepting use of forward declared class?
        //VAGUE_STATIC_ASSERT_MSG(std::is_base_of_v<Vague::IntrusiveRefCount, PtrType>, "This class must inherit from Vague::IntrusiveRefCount!");
        
    public:

        // Constructors
        SharedPtr()
            : PtrHandler<T>()
        {
        }

        explicit SharedPtr(PtrType* _p)
            : PtrHandler<T>(_p)
        {
            if (_p != nullptr)
            {
                _p->AddRef();
            }
        }

        template <class U>
        explicit SharedPtr(typename PtrHandler<U>::PtrType* _p)
            : PtrHandler<T>(_p)
        {
            if (_p != nullptr)
            {
                _p->AddRef();
            }
        }

        SharedPtr(const SharedPtr& _p)
            : PtrHandler<T>()
        {
            Reset(_p.m_ptr);
        }

        template <class U>
        explicit SharedPtr(const SharedPtr<U>& _p)
            : PtrHandler<T>()
        {
            VAGUE_STATIC_ASSERT_MSG(std::has_virtual_destructor_v<PtrType>, "Deleting from base class without virtual destructor is UndefinedBehaviour");
            Reset(_p.Get());
        }

        SharedPtr(SharedPtr&& _p)
            : PtrHandler<T>(std::forward<PtrHandler<T>>(_p))
        {
        }

        template <class U>
        explicit SharedPtr(SharedPtr<U>&& _p)
            : PtrHandler<T>(std::forward<PtrHandler<U>>(_p))
        {
            VAGUE_STATIC_ASSERT_MSG(std::has_virtual_destructor_v<PtrType>, "Deleting from base class without virtual destructor is UndefinedBehaviour");
        }

        // Destructor
        ~SharedPtr()
        {
            Release();
        }

        SharedPtr&  operator = (PtrType* _p)
        {
            Reset(_p);
            return *this;
        }

        SharedPtr&  operator = (const SharedPtr& _p)
        {
            return operator=(_p.m_ptr);
        }

        SharedPtr&  operator = (SharedPtr&& _p)
        {
            Swap(_p);
            return *this;
        }

        void Reset(PtrType* _p)
        {
            if (_p != m_ptr)
            {
                Release();
                m_ptr = _p;
                if (m_ptr != nullptr)
                {
                    m_ptr->AddRef();
                }
            }
        }

        void Release()
        {
            if (m_ptr != nullptr)
            {
                if (m_ptr->ReleaseRef() == 0)
                {
                    VAGUE_DELETE(m_ptr);
                }
                m_ptr = nullptr;
            }
        }

        void Swap(SharedPtr& _p)
        {
            PtrType* pTmp = _p.m_ptr;
            _p.m_ptr = m_ptr;
            m_ptr = pTmp;
        }
    };

    template <class T, class U>
    inline SharedPtr<T> StaticPointerCast(const SharedPtr<U> &_p)
    {
        return SharedPtr<T>(static_cast<T*>(_p.Get()));
    }

    template <class T, class U>
    inline SharedPtr<T> DynamicPointerCast(const SharedPtr<U> &_p)
    {
        return SharedPtr<T>(dynamic_cast<T*>(_p.Get()));
    }
}
