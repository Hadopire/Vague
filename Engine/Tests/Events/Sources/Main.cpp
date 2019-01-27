#include <Tests/Tests.hpp>

#include "Events/EventManager.hpp"
#include "Events/EventData.hpp"
#include "Delegate/Delegate.hpp"
#include "Pointers.hpp"

namespace Vague
{
    class EventDataA : public IEventData
    {
    public:
        bool m_TestOrder;

        EventDataA() : m_TestOrder(false) {}
        virtual const EventType GetEventType(void) const
        {
            return EventDataType<EventDataA>::Type();
        }
        virtual void Serialize(std::ostream& out) const {}
        virtual EventDataPtr Copy(void) const
        {
            return EventDataPtr(new EventDataA());
        }
        virtual const char* GetName(void) const
        {
            return "Event Data A";
        }
    };

    class EventDataB : public IEventData
    {
    public:
        bool m_TestOrder;

        EventDataB() : m_TestOrder(false) {}
        virtual const EventType GetEventType(void) const
        {
            return EventDataType<EventDataB>::Type();
        }
        virtual void Serialize(std::ostream& out) const {}
        virtual EventDataPtr Copy(void) const
        {
            return EventDataPtr(new EventDataB());
        }
        virtual const char* GetName(void) const
        {
            return "Event Data B";
        }
    };

    class EventDataC : public IEventData
    {
    public:
        bool m_TestOrder;

        EventDataC() : m_TestOrder(false) {}
        virtual const EventType GetEventType(void) const
        {
            return EventDataType<EventDataC>::Type();
        }
        virtual void Serialize(std::ostream& out) const {}
        virtual EventDataPtr Copy(void) const
        {
            return EventDataPtr(new EventDataC());
        }
        virtual const char* GetName(void) const
        {
            return "Event Data C";
        }
    };

    // if TestOrder is true, verify that the order of processing is A -> B -> C
    // assuming success[A,B,C] are equal before processing.
    class Listeners
    {
    public:
        static int successA;
        static int successB;
        static int successC;

        static void ListenerA(EventDataPtr _data)
        {
            if(_data->GetEventType() == EventDataType<EventDataA>::Type())
                ++successA;

            SharedPtr<EventDataA> data = StaticPointerCast<EventDataA>(_data);
            if (data->m_TestOrder)
            {
                if (successA <= successB || successA <= successC)
                    --successA;
            }
        }

        static void ListenerB(EventDataPtr _data)
        {
            if(_data->GetEventType() == EventDataType<EventDataB>::Type())
                ++successB;

            SharedPtr<EventDataB> data = StaticPointerCast<EventDataB>(_data);
            if (data->m_TestOrder)
            {
                if (successB != successA || successB <= successC)
                    --successB;
            }
        }

        static void ListenerC(EventDataPtr _data)
        {
            if(_data->GetEventType() == EventDataType<EventDataC>::Type())
                ++successC;

            SharedPtr<EventDataC> data = StaticPointerCast<EventDataC>(_data);
            if (data->m_TestOrder)
            {
                if (successC != successA || successC != successB)
                    --successC;
            }
        }
    };
    int Listeners::successA = 0;
    int Listeners::successB = 0;
    int Listeners::successC = 0;

    u32 EventManagerTest()
    {
        VAGUE_UTEST_INIT;

        EventManager *eventManager = EventManager::Get();
        eventManager->AddListener(EventDataType<EventDataA>::Type(), MakeDelegate(Listeners::ListenerA));
        eventManager->AddListener(EventDataType<EventDataB>::Type(), MakeDelegate(Listeners::ListenerB));
        eventManager->AddListener(EventDataType<EventDataC>::Type(), MakeDelegate(Listeners::ListenerC));

        EventDataPtr eventDataA(new EventDataA);
        EventDataPtr eventDataB(new EventDataB);
        EventDataPtr eventDataC(new EventDataC);

        // Process one event A
        eventManager->Queue(eventDataA->GetEventType(), eventDataA);
        eventManager->ProcessQueue();
        VAGUE_UTEST(Listeners::successA == 1);
        VAGUE_UTEST(Listeners::successB == 0);
        VAGUE_UTEST(Listeners::successC == 0);

        // Process no event
        eventManager->ProcessQueue();
        VAGUE_UTEST(Listeners::successA == 1);
        VAGUE_UTEST(Listeners::successB == 0);
        VAGUE_UTEST(Listeners::successC == 0);

        // Process two B and one C
        eventManager->Queue(eventDataB->GetEventType(), eventDataB);
        eventManager->Queue(eventDataC->GetEventType(), eventDataC);
        eventManager->Queue(eventDataB->GetEventType(), eventDataB);
        VAGUE_UTEST(eventDataB->GetRefCount() == 3);
        VAGUE_UTEST(eventDataC->GetRefCount() == 2);
        eventManager->ProcessQueue();
        VAGUE_UTEST(Listeners::successA == 1);
        VAGUE_UTEST(Listeners::successB == 2);
        VAGUE_UTEST(Listeners::successC == 1);

        // Verify that the manager correctly releases pointers
        VAGUE_UTEST(eventDataA->GetRefCount() == 1);
        VAGUE_UTEST(eventDataB->GetRefCount() == 1);
        VAGUE_UTEST(eventDataC->GetRefCount() == 1);

        // Testing processing order
        Listeners::successA = 0;
        Listeners::successB = 0;
        Listeners::successC = 0;
        StaticPointerCast<EventDataA>(eventDataA)->m_TestOrder = 1;
        StaticPointerCast<EventDataB>(eventDataB)->m_TestOrder = 1;
        StaticPointerCast<EventDataC>(eventDataC)->m_TestOrder = 1;
        eventManager->Queue(eventDataA->GetEventType(), eventDataA);
        eventManager->Queue(eventDataB->GetEventType(), eventDataB);
        eventManager->Queue(eventDataC->GetEventType(), eventDataC);
        eventManager->ProcessQueue();
        VAGUE_UTEST(Listeners::successA == 1);
        VAGUE_UTEST(Listeners::successB == 1);
        VAGUE_UTEST(Listeners::successC == 1);
        StaticPointerCast<EventDataA>(eventDataA)->m_TestOrder = 0;
        StaticPointerCast<EventDataB>(eventDataB)->m_TestOrder = 0;
        StaticPointerCast<EventDataC>(eventDataC)->m_TestOrder = 0;

        VAGUE_UTEST_END;
    }
}

int main()
{
    Vague::EventManagerTest();
    return 0;
}
