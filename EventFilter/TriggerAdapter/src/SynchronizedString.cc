#include "EventFilter/TriggerAdapter/interface/SynchronizedString.h"


rubuilder::ta::SynchronizedString::SynchronizedString() :
bSem_(toolbox::BSem::FULL)
{
}


void rubuilder::ta::SynchronizedString::setValue(std::string value)
{
    bSem_.take();
    value_ = value;
    bSem_.give();
}


void rubuilder::ta::SynchronizedString::setValue(const char *value)
{
    bSem_.take();
    value_ = value;
    bSem_.give();
}


std::string rubuilder::ta::SynchronizedString::getValue()
{
    bSem_.take();
    std::string value = value_;
    bSem_.give();

    return value;
}
