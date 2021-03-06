
#include <Tests/Tests.hpp>
#include <Pointers.hpp>
#include <Output.hpp>

// Uncomment it to print things
//#define VERBOSE_TEST 1

namespace Vague
{
    struct ZeroWhenDestroyed
    {
        ZeroWhenDestroyed(u32& _ref)
            : ref(_ref)
        {
        }

        ~ZeroWhenDestroyed()
        {
            ref = 0;
        }

        u32& ref;
    };

    struct Base
    {
        virtual ~Base() {}
        int i = 0;
    };
    struct Derived : public Base
    {
        int j = 0;
    };
    struct NotDerived
    {
        int j = 0;
    };

    u32 Tests_Unique()
    {
        VAGUE_UTEST_INIT;

        u32 i = 12;

        {
            UniquePtr<ZeroWhenDestroyed> pUPtr{ new ZeroWhenDestroyed{ i } };  // by raw pointer constructor
            pUPtr.Release();
            VAGUE_UTEST(i == 0);
        }

        i = 12;
        {
            UniquePtr<ZeroWhenDestroyed> pUPtr{ new ZeroWhenDestroyed{ i } };
        }
        VAGUE_UTEST(i == 0);

        {
            UniquePtr<u32> pUPtr01;
            pUPtr01 = new u32;
            VAGUE_UTEST(pUPtr01.Get() != nullptr);
            {
                //UniquePtr<u32> pUPtr02{ pUPtr01 }; // copy constructor, must NOT compile if uncommented
                //pUPtr01 = UniquePtr<u32>{};        // copy assignement, must NOT compile if uncommented
            }
            {
                UniquePtr<u32> pUPtr02{ std::move(pUPtr01) }; // move constructor
                VAGUE_UTEST(pUPtr01.Get() == nullptr);
                VAGUE_UTEST(pUPtr02.Get() != nullptr);
            }
        }

        {
            UniquePtr<Derived> pUPtrDerived{ new Derived() };
            UniquePtr<Base> pUPtrBase{ std::move(pUPtrDerived) }; // move constructor
            VAGUE_UTEST(pUPtrDerived.Get() == nullptr);
            VAGUE_UTEST(pUPtrBase.Get() != nullptr);
        }

        {
            UniquePtr<Base> pUPtrBase{ new Base() };
            //UniquePtr<Derived> pUPtrDerived{ std::move(pUPtrBase) }; // must NOT compile if uncommented
        }

        {
            UniquePtr<NotDerived> pUPtrNotDerived{ new NotDerived() };
            //UniquePtr<Base> pUPtrBase{ std::move(pUPtrNotDerived) }; // must NOT compile if uncommented
        }

        VAGUE_UTEST_END;
    }

    struct IntrusivePrint
        : public IntrusiveRefCount
    {
        IntrusivePrint()
        {
            #if defined (VERBOSE_TEST)
            VAGUE_COUT << "O IntrusivePrint constructor" << std::endl;
            #endif
        }

        ~IntrusivePrint()
        {
            #if defined (VERBOSE_TEST)
            VAGUE_COUT << "X IntrusivePrint destructor" << std::endl;
            #endif
        }

        u32 i = 12;
    };

    struct NotIntrusive
    {
        u32 i = 13;
    };

    struct IntrusiveNotPrint
        : public IntrusiveRefCount
    {
    };

    u32 Tests_Shared()
    {
        VAGUE_UTEST_INIT;

        //SharedPtr<NotIntrusive> pNope{ new NotIntrusive }; // Must NOT compile if uncommented

        {
            SharedPtr<IntrusivePrint> pSPtr;
            VAGUE_UTEST(pSPtr.Get() == nullptr);
        }

        {
            IntrusivePrint* pIntrusive = new IntrusivePrint;
            VAGUE_UTEST(pIntrusive->GetRefCount() == 0);
            pIntrusive->AddRef();
            VAGUE_UTEST(pIntrusive->GetRefCount() == 1);
            pIntrusive->ReleaseRef();
            VAGUE_UTEST(pIntrusive->GetRefCount() == 0);
            delete pIntrusive;
        }

        {
            SharedPtr<IntrusivePrint> pSPtr01;
            pSPtr01 = new IntrusivePrint;
            VAGUE_UTEST(pSPtr01->GetRefCount() == 1);
            {
                SharedPtr<IntrusivePrint> pSPtr02{ pSPtr01 }; // copy constructor
                VAGUE_UTEST(pSPtr01.Get() == pSPtr02.Get());
                VAGUE_UTEST(pSPtr01->GetRefCount() == 2);
                VAGUE_UTEST(pSPtr02->GetRefCount() == 2);
                pSPtr02.Release();
                VAGUE_UTEST(pSPtr01->GetRefCount() == 1);
                VAGUE_UTEST(pSPtr02.Get() == nullptr);
            }
            {
                SharedPtr<IntrusivePrint> pSPtr02{ pSPtr01.Get() }; // by raw pointer constructor
                VAGUE_UTEST(pSPtr01.Get() == pSPtr02.Get());
                VAGUE_UTEST(pSPtr01->GetRefCount() == 2);
                VAGUE_UTEST(pSPtr02->GetRefCount() == 2);
            }
            VAGUE_UTEST(pSPtr01->GetRefCount() == 1);

            pSPtr01.Release();
            VAGUE_UTEST(pSPtr01.Get() == nullptr);
        }

        {
            SharedPtr<IntrusivePrint> pSPtr01;
            pSPtr01 = new IntrusivePrint;
            VAGUE_UTEST(pSPtr01->GetRefCount() == 1);
            {
                SharedPtr<IntrusivePrint> pSPtr02{ std::move(pSPtr01) }; // move constructor
                VAGUE_UTEST(pSPtr01.Get() == nullptr);
                VAGUE_UTEST(pSPtr02.Get() != nullptr);
                VAGUE_UTEST(pSPtr02->GetRefCount() == 1);
                pSPtr02.Release();
            }
        }


        {
            SharedPtr<IntrusivePrint> pSPtrDerived{ new IntrusivePrint() };
            SharedPtr<IntrusiveRefCount> pSPtrBase{ pSPtrDerived }; // copy constructor
            VAGUE_UTEST(pSPtrDerived.Get() == pSPtrBase.Get());
            VAGUE_UTEST(pSPtrDerived->GetRefCount() == 2);
            VAGUE_UTEST(pSPtrBase->GetRefCount() == 2);
        }
        {
            SharedPtr<IntrusivePrint> pSPtrDerived{ new IntrusivePrint() };
            SharedPtr<IntrusiveRefCount> pSPtrBase{ std::move(pSPtrDerived) }; // move constructor
            VAGUE_UTEST(pSPtrDerived.Get() == nullptr);
            VAGUE_UTEST(pSPtrBase.Get() != nullptr);
            VAGUE_UTEST(pSPtrBase->GetRefCount() == 1);
        }

        {
            SharedPtr<IntrusiveRefCount> pSPtrBase{ new IntrusiveRefCount() };
            //SharedPtr<IntrusivePrint> pSPtrDerived{ std::move(pSPtrBase) }; // must NOT compile if uncommented
        }

        {
            SharedPtr<IntrusiveNotPrint> pSPtrNotDerived{ new IntrusiveNotPrint() };
            //SharedPtr<IntrusivePrint> pSPtrBase{ std::move(pSPtrNotDerived) }; // must NOT compile if uncommented
        }

        VAGUE_UTEST_END;
    }
}

int main()
{
    Vague::Tests_Unique();
    Vague::Tests_Shared();

    return 0;
}
