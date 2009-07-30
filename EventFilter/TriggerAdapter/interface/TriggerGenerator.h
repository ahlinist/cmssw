#ifndef _rubuilder_ta_TriggerGenerator_h_
#define _rubuilder_ta_TriggerGenerator_h_

#include "EventFilter/TriggerAdapter/interface/Exception.h"
#include "i2o/i2oDdmLib.h"
#include "i2o/Method.h"
#include "interface/evb/i2oEVBMsgs.h"
#include "interface/shared/i2oXFunctionCodes.h"
#include "toolbox/mem/MemoryPoolFactory.h"
#include "toolbox/mem/Pool.h"
#include "xcept/Exception.h"

#include <stdint.h>


namespace rubuilder { namespace ta { // namespace rubuilder::ta

/**
 * Generates dummy triggers.
 */
class TriggerGenerator
{
public:

    /**
     * Generates and returns a single dummy I2O_EVM_TRIGGER message.
     *
     * The FED payload size is in bytes and must be a multiple of 8 bytes.
     */
    toolbox::mem::Reference *generate
    (
        toolbox::mem::MemoryPoolFactory *poolFactory,
        toolbox::mem::Pool              *pool,
        const I2O_TID                   initiatorAddress,
        const I2O_TID                   targetAddress,
        const uint32_t                  triggerSourceId,
        const U32                       eventNumber,
        const size_t                    fedPayloadSize
    )
    throw (ta::exception::Exception);
};

} } // namespace rubuilder::ta

#endif
