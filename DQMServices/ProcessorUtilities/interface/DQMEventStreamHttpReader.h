// $Id: EventStreamHttpReader.h,v 1.29 2011/09/02 08:13:31 momsen Exp $
/// @file: DQMEventStreamHttpReader.h
#ifndef ProcessorUtilities_DQMEventStreamHttpReader_h
#define ProcessorUtilities_DQMEventStreamHttpReader_h

#include "DQMServices/Core/interface/DQMStore.h"
#include "EventFilter/StorageManager/interface/EventConsumerRegistrationInfo.h"
#include "EventFilter/StorageManager/interface/EventServerProxy.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "IOPool/Streamer/interface/StreamerInputSource.h"

#include "DQMServices/ProcessorUtilities/interface/InputController.h"

namespace edm
{
  /**
    Input source for event consumers that will get events from the
    Storage Manager Event Server. This does uses a HTTP get using the
    CURL library. The Storage Manager Event Server responses with
    a binary octet-stream.  The product registry is also obtained
    through a HTTP get.

    There is currently no test of the product registry against
    the consumer client product registry within the code. It should
    already be done if this was inherenting from the standard
    framework input source. Currently we inherit from InputSource.

    $Author: mommsen $
    $Revision: 1.29 $
    $Date: 2011/09/02 08:13:31 $
  */

  class DQMEventStreamHttpReader : public edm::StreamerInputSource, private dqmevf::InputController
  {
  public:
    DQMEventStreamHttpReader
    (
      edm::ParameterSet const&,
      edm::InputSourceDescription const&
    );
    virtual ~DQMEventStreamHttpReader() {};

    virtual EventPrincipal* read();

  private:
    void readHeader();
    void initializeDQMStore();
    
    stor::EventServerProxy<stor::EventConsumerRegistrationInfo> eventServerProxy_;
    DQMStore* dqmStore_;
    bool dqmStoreAvailabiltyChecked_;

    bool dropOldLumisectionEvents_;
    const std::string consumerName_;
    unsigned int totalDroppedEvents_;
    unsigned int lastLS_;
    bool registered_;
    bool registeredFirst_;
    unsigned long lastEID_;
    unsigned long useMinEID_;

    unsigned int shutdown_;

    //members for the dqmevf::Processor synchronisation
    void initFUHelpers();
    void publish(dqmevf::evfSourceVars * vars);
    void trapSource(unsigned int timeout_sec);
    void untrapSource();
    bool lumiSyncHandler(unsigned int);
    void signalWaitingThreadAndBlock();
    void unblockScalers();
    dqmevf::evfSourceVars *evfVars_;
    pthread_mutex_t mutex_;
    pthread_mutex_t signal_lock_;
    pthread_cond_t cond_;


  };

} // namespace edm

#endif // ProcessorUtilities_DQMEventStreamHttpReader_h

/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
