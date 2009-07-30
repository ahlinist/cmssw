#include "EventFilter/TriggerAdapter/interface/TriggerGenerator.h"
#include "interface/shared/frl_header.h"
#include "interface/shared/fed_header.h"
#include "interface/shared/fed_trailer.h"
#include "toolbox/mem/MemoryPoolFactory.h"


toolbox::mem::Reference *rubuilder::ta::TriggerGenerator::generate
(
    toolbox::mem::MemoryPoolFactory *poolFactory,
    toolbox::mem::Pool              *pool,
    const I2O_TID                   initiatorAddress,
    const I2O_TID                   targetAddress,
    const uint32_t                  triggerSourceId,
    const U32                       eventNumber,
    const size_t                    fedPayloadSize
)
throw (rubuilder::ta::exception::Exception)
{
    toolbox::mem::Reference            *bufRef     = 0;
    I2O_MESSAGE_FRAME                  *stdMsg     = 0;
    I2O_PRIVATE_MESSAGE_FRAME          *pvtMsg     = 0;
    I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME *block      = 0;
    size_t                             bufSize     = 0;
    char                               *payload    = 0;
    frlh_t                             *frlHeader  = 0;
    fedh_t                             *fedHeader  = 0;
    fedt_t                             *fedTrailer = 0;


    if(fedPayloadSize % 4 != 0)
    {
        std::stringstream oss;

        oss << "The requested FED payload of " << fedPayloadSize << " bytes";
        oss << " is not a multiple of 8 bytes";

        XCEPT_RAISE(rubuilder::ta::exception::Exception, oss.str());
    }

    bufSize = sizeof(I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME) + // RU builder header
              sizeof(frlh_t)                             + // FRL header
              sizeof(fedh_t)                             + // FED header
              fedPayloadSize                             + // FED payload
              sizeof(fedt_t);                              // FED trailer


    //////////////////////////////////////////////////
    // Allocate a buffer for the trigger data frame //
    //////////////////////////////////////////////////

    try
    {
        bufRef = poolFactory->getFrame(pool, bufSize);
        bufRef->setDataSize(bufSize);
    }
    catch(xcept::Exception e)
    {
        XCEPT_RETHROW(rubuilder::ta::exception::Exception,
                      "Failed to allocate I2O_EVM_TRIGGER message", e);
    }


    /////////////////////
    // Fill with zeros //
    /////////////////////

//  ::memset(bufRef->getDataLocation(), 0,  bufSize);


    //////////////////////////////////////////////////
    // Fill in the fields of the trigger data frame //
    //////////////////////////////////////////////////

    stdMsg = (I2O_MESSAGE_FRAME*)bufRef->getDataLocation();
    pvtMsg = (I2O_PRIVATE_MESSAGE_FRAME*)stdMsg;
    block  = (I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME*)stdMsg;

    stdMsg->MessageSize      = bufSize >> 2;
    stdMsg->InitiatorAddress = initiatorAddress;
    stdMsg->TargetAddress    = targetAddress;
    stdMsg->Function         = I2O_PRIVATE_MESSAGE;
    stdMsg->VersionOffset    = 0;
    stdMsg->MsgFlags         = 0;  // Point-to-point

    pvtMsg->XFunctionCode    = I2O_EVM_TRIGGER;
    pvtMsg->OrganizationID   = XDAQ_ORGANIZATION_ID;

    block->eventNumber             = eventNumber;
    block->nbBlocksInSuperFragment = 1;
    block->blockNb                 = 0;

    payload = ((char*)block) + sizeof(I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME);

    frlHeader   = (frlh_t*)(payload);
    fedHeader   = (fedh_t*)(payload + sizeof(frlh_t));
    fedTrailer  = (fedt_t*)(payload + sizeof(frlh_t) + sizeof(fedh_t) +
        fedPayloadSize);

    frlHeader->source     = 0;
    frlHeader->trigno     = eventNumber;
    frlHeader->segno      = 0;
    frlHeader->reserved   = 0;
    frlHeader->segsize    = FRL_LAST_SEGM | (sizeof(fedh_t) + fedPayloadSize +
        sizeof(fedt_t));
    frlHeader->reserved_2 = 0;

    fedHeader->sourceid   = triggerSourceId << FED_SOID_SHIFT;
    fedHeader->eventid    = (FED_SLINK_START_MARKER << FED_HCTRLID_SHIFT)
        | eventNumber;

    fedTrailer->conscheck = 0xDEADFACE;
    fedTrailer->eventsize = (FED_SLINK_END_MARKER << FED_HCTRLID_SHIFT) |
        ((sizeof(fedh_t) + fedPayloadSize + sizeof(fedt_t)) >> 3);

    return bufRef;
}
