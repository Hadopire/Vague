#include "Events/EventData.hpp"

namespace Vague
{
    EventType GetNextEventType()
    {
        static EventType nextEventType = 0;
        return nextEventType++;
    }
}
