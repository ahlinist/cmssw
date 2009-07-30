#ifndef _rubuilder_ta_SynchronizedString_h_
#define _rubuilder_ta_SynchronizedString_h_

#include "toolbox/BSem.h"

#include <string>


namespace rubuilder { namespace ta { // namespace rubuilder::ta

/**
 * A thread-safe string class.
 */
class SynchronizedString
{
private:

    /**
     * The value of the string.
     */
    std::string value_;

    /**
     * Binary semaphore used to protect the value of the string.
     */
    toolbox::BSem bSem_;


public:


    /**
     * Constructor.
     */
    SynchronizedString();

    /**
     * Sets the value of the string.
     */
    void setValue(std::string value);

    /**
     * Sets the value of the string.
     */
    void setValue(const char *value);

    /**
     * Gets the value of the string.
     */
    std::string getValue();
};

} } // namespace rubuilder::ta

#endif
