#pragma once

#include "Core.hpp"
#include "ClassTraits.hpp"
#include "Types.hpp"

#include <type_traits>


namespace Vague
{
    // Smart pointers base class (do NOT use it)

    #define VAGUE_STATIC_ASSERT_NOT_ARRAY(_Class)   VAGUE_STATIC_ASSERT_MSG((!std::is_array_v<_Class>), "Do not use smart pointers with arrays!")

    template <class T>
    class BaseSmartPtr
    {
        VAGUE_STATIC_ASSERT_NOT_ARRAY(T);

    public:

        using PtrType = std::remove_extent_t<T>;

        // Constructors
        BaseSmartPtr()
            : m_ptr(nullptr)
        {
        }

        BaseSmartPtr(BaseSmartPtr&& _p)
            : m_ptr(_p.m_ptr)
        {
            _p.m_ptr = nullptr;
        }

        // Destructor
        ~BaseSmartPtr()
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
        : public BaseSmartPtr<T>
    {
        VAGUE_NONCOPYABLE(UniquePtr);

    public:

        // Constructors
        UniquePtr()
            : BaseSmartPtr<T>()
        {
        }

        explicit UniquePtr(PtrType* _p)
            : BaseSmartPtr<T>()
        {
            Reset(_p);
        }

        UniquePtr(UniquePtr&& _p)
            : BaseSmartPtr<T>(std::forward<BaseSmartPtr<T>>(_p))
        {
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

        IntrusiveRefCount() = default;
        ~IntrusiveRefCount()
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
        : public BaseSmartPtr<T>
    {
        VAGUE_STATIC_ASSERT_MSG(std::is_base_of_v<Vague::IntrusiveRefCount, PtrType>, "This class must inherit from Vague::IntrusiveRefCount!");

    public:

        // Constructors
        SharedPtr()
            : BaseSmartPtr<T>()
        {
        }

        SharedPtr(PtrType* _p)
            : BaseSmartPtr<T>()
        {
            Reset(_p);
        }

        SharedPtr(const SharedPtr& _p)
            : SharedPtr(_p.m_ptr)
        {
        }

        SharedPtr(SharedPtr&& _p)
            : BaseSmartPtr<T>(std::forward<BaseSmartPtr<T>>(_p))
        {
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
}
