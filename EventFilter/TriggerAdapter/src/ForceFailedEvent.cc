#include "EventFilter/TriggerAdapter/interface/ForceFailedEvent.h"


rubuilder::ta::ForceFailedEvent::ForceFailedEvent
(
    const std::string &reason,
    void              *originator
) :
toolbox::Event("Fail", originator)
{
    reason_ = reason;
}


std::string rubuilder::ta::ForceFailedEvent::getReason()
{
    return reason_;
}
