#pragma once

#include <ostream>

#include "Types.hpp"
#include "Pointers.hpp"

namespace Vague
{
    typedef u64 EventType;

    class IEventData;
    typedef SharedPtr<IEventData> EventDataPtr;

    class IEventData : public IntrusiveRefCount
    {
    public:
        virtual ~IEventData() {}

        virtual const EventType GetEventType() const = 0;
        virtual void Serialize(std::ostream &_out) const = 0;
        virtual EventDataPtr Copy() const = 0;
        virtual const char* GetName() const = 0;
    };

    EventType GetNextEventType();

    template <class T,
             class = typename std::enable_if_t<std::is_base_of<IEventData, T>::value, void>>
    struct EventDataType
    {
        static EventType Type()
        {
            static const EventType type = GetNextEventType();
            return type;
        }
    };
}
