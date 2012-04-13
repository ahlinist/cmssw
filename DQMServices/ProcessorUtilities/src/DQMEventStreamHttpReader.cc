// $Id: DQMEventStreamHttpReader.cc,v 1.52 2011/09/02 08:13:31 mommsen Exp $
/// @file: DQMEventStreamHttpReader.cc

#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/ProcessorUtilities/interface/DQMEventStreamHttpReader.h"
#include "EventFilter/StorageManager/interface/CurlInterface.h"
#include "EventFilter/StorageManager/src/EventServerProxy.icc"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"

#include <string>


namespace edm
{  
  DQMEventStreamHttpReader::DQMEventStreamHttpReader
  (
    ParameterSet const& pset,
    InputSourceDescription const& desc
  ):
  StreamerInputSource(pset, desc),
  dqmevf::InputController("HTTPInputSource"),
  eventServerProxy_(pset),
  dqmStore_(0),
  dqmStoreAvailabiltyChecked_(false),
  dropOldLumisectionEvents_(pset.getUntrackedParameter<bool>("dropOldLumisectionEvents", false)),
  consumerName_(pset.getUntrackedParameter<std::string>("consumerName")),
  totalDroppedEvents_(0),
  lastLS_(0),
  registered_(0),
  registeredFirst_(0),
  lastEID_(0),
  useMinEID_(0),
  shutdown_(0)
  {
    // Default in StreamerInputSource is 'false'
    inputFileTransitionsEachEvent_ =
      pset.getUntrackedParameter<bool>("inputFileTransitionsEachEvent", true);

    eventServerProxy_.setShutdownPtr(&shutdown_);
    initFUHelpers();
    readHeader();
  }

  EventPrincipal* DQMEventStreamHttpReader::read()
  {
    initializeDQMStore();

    stor::CurlInterface::Content data;
    unsigned int currentLS(0);
    unsigned int droppedEvents(0);

    do
    {
      while (!registered_ && !edm::shutdown_flag && !shutdown_) {
        try {
	  eventServerProxy_.reconnect();
	 readHeader();
        }
        catch (...) {std::cout << "EventStreamHTTPReader RECONNECT: exception"<<std::endl;}
      }
      eventServerProxy_.getOneEvent(data);
      //end of run
      if (edm::shutdown_flag || shutdown_)
      {
	unblockScalers();
	setEndRun();
	return 0;
      }
      //should catch this earlier
      if ( data.empty() ) return 0;
      HeaderView hdrView(&data[0]);
      if (hdrView.code() == Header::DONE)
      {
        if (!evfVars_) {
          setEndRun();
          return 0;
        }
	registered_=false;
	useMinEID_=lastEID_+1;
	continue;
      }
      EventMsgView eventView(&data[0]);
      currentLS = eventView.lumi();
      lastEID_ = eventView.event();
      droppedEvents += eventView.droppedEventsCount();
    }
    while (( lastEID_ >= useMinEID_ && !lumiSyncHandler(currentLS))
	|| lastEID_ < useMinEID_ //skip events that might be reprocessed after SMPS restart
	);

    if (evfVars_) evfVars_->processedEvents++;

    lastLS_ = currentLS;

    if (dqmStore_)
    {
      MonitorElement* me = dqmStore_->get("SM_SMPS_Stats/droppedEventsCount_" + consumerName_ );
      if (!me){
        dqmStore_->setCurrentFolder("SM_SMPS_Stats");
        me = dqmStore_->bookInt("droppedEventsCount_" + consumerName_ );
      }
      totalDroppedEvents_ += droppedEvents;
      me->Fill(totalDroppedEvents_);
    }

    return deserializeEvent(EventMsgView(&data[0]));
  }
  
  
  void DQMEventStreamHttpReader::readHeader()
  {
    stor::CurlInterface::Content data;
    if (registered_) return; 
    eventServerProxy_.getInitMsg(data);
    if (registeredFirst_) {
      registered_=true;
      return;
    }
    InitMsgView initView(&data[0]);
    deserializeAndMergeWithRegistry(initView);
    registeredFirst_=true;
    registered_=true;
  }
  
  
  void DQMEventStreamHttpReader::initializeDQMStore()
  {
    if ( ! dqmStoreAvailabiltyChecked_ )
    {   
      try
      {
        dqmStore_ = edm::Service<DQMStore>().operator->();
      }
      catch (cms::Exception& e)
      {
        edm::LogInfo("DQMEventStreamHttpReader")
          << "Service DQMStore not defined. Will not record the number of dropped events.";
      }
      dqmStoreAvailabiltyChecked_ = true;
    }
  }

  void DQMEventStreamHttpReader::initFUHelpers()
  {
    evfVars_ = 0;
    pthread_mutex_init(&mutex_,0);
    pthread_mutex_init(&signal_lock_,0);
    pthread_cond_init(&cond_,0);
  }

  void DQMEventStreamHttpReader::publish(dqmevf::evfSourceVars *vars)
  {
    pthread_mutex_init(&mutex_,0);
    pthread_mutex_init(&signal_lock_,0);
    pthread_cond_init(&cond_,0);
    evfVars_=vars;
    evfVars_->runStopFlag=&shutdown_;
    dropOldLumisectionEvents_=vars->dropOldLS;
  }

  void DQMEventStreamHttpReader::trapSource(unsigned int timeout_sec)
  {
    if (evfVars_==0) return;
    evfVars_->trappingCounter++;
    pthread_mutex_lock(&mutex_);
    pthread_mutex_unlock(&signal_lock_);
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout_sec;
    int rc = pthread_cond_timedwait(&cond_, &mutex_, &ts);
    if(rc == ETIMEDOUT) evfVars_->hasTimedOut = true;
    else evfVars_->hasTimedOut=false;
  }

  void DQMEventStreamHttpReader::untrapSource()
  {
    if (evfVars_==0) return;
    evfVars_->trappingCounter--;
    pthread_cond_signal(&cond_);
    pthread_mutex_unlock(&mutex_);
    pthread_mutex_lock(&signal_lock_);
    evfVars_->hasTimedOut=false;
  }

  void DQMEventStreamHttpReader::signalWaitingThreadAndBlock()
  {
    if (evfVars_==0) return;
    pthread_mutex_lock(&signal_lock_);
    pthread_mutex_lock(&mutex_);
    pthread_mutex_unlock(&signal_lock_);
    pthread_cond_signal(&cond_);
    pthread_cond_wait(&cond_, &mutex_);
    pthread_mutex_unlock(&mutex_);
    ::usleep(1000);//allow other thread to lock
  }

  void DQMEventStreamHttpReader::unblockScalers()
  {
    if (evfVars_==0) return;
    pthread_mutex_lock(&mutex_);
    pthread_cond_signal(&cond_);
    pthread_mutex_unlock(&mutex_);
  }

  bool DQMEventStreamHttpReader::lumiSyncHandler(unsigned int currentLS)
  {
    if (!currentLS) return true;
    //set real lumi of last received event (even if late)
    if (evfVars_) evfVars_->evtLumi=currentLS;
    //sync with FWEP thread for LS report
    if (evfVars_ && currentLS>lastLS_) {
      evfVars_->lumi=currentLS;
      evfVars_->lastLumi=lastLS_;
      signalWaitingThreadAndBlock();
    }
    //skip event if late LS
    if (currentLS<lastLS_ && dropOldLumisectionEvents_) {
      if (evfVars_) evfVars_->droppedEvents++;
      return false;
    }

    //update value
    if (lastLS_==0 || currentLS>lastLS_) {
      if (evfVars_) {
       evfVars_->lumi=currentLS;
       evfVars_->lastLumi=lastLS_;
      }
      lastLS_=currentLS;
    }
    return true;
  }


} //namespace edm


/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
