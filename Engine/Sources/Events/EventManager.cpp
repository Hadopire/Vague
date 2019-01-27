#include "Events/EventManager.hpp"

#include "Core.hpp"

namespace Vague
{
    EventManager* EventManager::Get()
    {
        static EventManager eventManager;
        return &eventManager;
    }

    bool EventManager::AddListener(const EventType _eventType, const EventListener &_listener)
    {
        std::list<EventListener> &listenerList = m_eventListenerMap[_eventType];
        for (auto it = listenerList.begin(); it != listenerList.end(); ++it)
        {
            if (*it == _listener)
            {
                VAGUE_TODO("Hadopire", "Log this in debug build");
                return false;
            }
        }
        listenerList.push_back(_listener);
        return true;
    }

    bool EventManager::RemoveListener(const EventType _eventType, const EventListener &_listener)
    {
        std::list<EventListener> &listenerList = m_eventListenerMap[_eventType];
        for (auto it = listenerList.begin(); it != listenerList.end(); ++it)
        {
            if (*it == _listener)
            {
                listenerList.erase(it);
                return true;
            }
        }
        VAGUE_TODO("Hadopire", "Log this in debug build");
        return false;
    }

    bool EventManager::Queue(const EventType _eventType, const EventDataPtr _eventData)
    {
        if (!m_eventListenerMap.count(_eventType))
        {
            return false;
        }
        m_eventQueue[m_activeQueue].push_back(_eventData);
        return true;
    }

    bool EventManager::Cancel(const EventType _eventType, bool _all)
    {
        bool success = false;
        auto it = m_eventQueue[m_activeQueue].begin();
        while (it != m_eventQueue[m_activeQueue].end())
        {
            EventType eventType = (*it)->GetEventType();
            if (eventType == _eventType)
            {
                success = true;
                m_eventQueue[m_activeQueue].erase(it++);
                if (!_all)
                    break;
            }
            else
            {
                ++it;
            }
        }
        return success;
    }

    void EventManager::ProcessQueue()
    {
        size_t processedQueue = m_activeQueue;
        m_activeQueue = (m_activeQueue + 1) % EVMANAGER_NUM_QUEUE;

        for (EventDataPtr eventData : m_eventQueue[processedQueue])
        {
            EventType eventType = eventData->GetEventType();
            for (EventListener &listener : m_eventListenerMap[eventType])
            {
                listener(eventData);
            }
        }
        m_eventQueue[processedQueue].clear();
    }
}
