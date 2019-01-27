#pragma once

#include <functional>
#include <unordered_map>
#include <list>

#include "Events/EventData.hpp"
#include "Delegate/Delegate.hpp"

#define EVMANAGER_NUM_QUEUE 2

namespace Vague
{
    typedef Delegate<void (EventDataPtr)> EventListener;

    class EventManager
    {
    public:
        EventManager()
            : m_activeQueue(0)
        {
        }

        static EventManager* Get();

        bool AddListener(const EventType _eventType, const EventListener &_listener);
        bool RemoveListener(const EventType _eventType, const EventListener &_listener);

        bool Queue(const EventType _eventType, const EventDataPtr _eventData);
        bool Cancel(const EventType _eventType, bool _all = false);

        void ProcessQueue();
    private:
        std::unordered_map<EventType, std::list<EventListener>> m_eventListenerMap;
        std::list<EventDataPtr>                                 m_eventQueue[EVMANAGER_NUM_QUEUE];

        size_t m_activeQueue;
    };
}
