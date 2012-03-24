#ifndef DQMEVF_FWEPWRAPPER_H
#define DQMEVF_FWEPWRAPPER_H

//fwk
#include "FWCore/PluginManager/interface/ProblemTracker.h"
#include "FWCore/PluginManager/interface/PresenceFactory.h"
#include "FWCore/Utilities/interface/Presence.h"

#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#undef HAVE_STAT
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/Framework/interface/TriggerReport.h"
#include "FWCore/Framework/interface/EventProcessor.h"

//xdaq
#include "xdaq/Application.h"
#include "toolbox/lang/Class.h"
#include "toolbox/task/WorkLoop.h"
#include "toolbox/net/URL.h"

#include "xgi/Input.h"
#include "xgi/Output.h"
#include "xgi/exception/Exception.h"
#include "xdata/InfoSpace.h"
#include "log4cplus/logger.h"

//dqmevf
#include "DQMServices/ProcessorUtilities/interface/MSService.h"
#include "DQMServices/ProcessorUtilities/interface/InputControllerRegistry.h"
#include "DQMServices/ProcessorUtilities/interface/InputController.h"
//local
#include "Exception.h"
#include "TriggerReportHelpers.h"
#include "MsgBuf.h"
#include "SubProcess.h"

//std
#include <string>
//boost

#include <sys/sem.h>
#include <time.h>

namespace dqmevf{

  struct LSInfo{
    LSInfo(): ls(0), proc(0), acc(0){}
    unsigned int ls;
    unsigned int proc;
    unsigned int acc;
  };


  struct WrapperConfig {

	  SubProcess * sub;
	  unsigned int slot;
	  
          xdaq::Application *xdaqApp;
	  xdata::InfoSpace *ispace;
	  std::string urn;
	  
	  std::string configPathTable;
	  boost::shared_ptr<edm::ProcessDesc> pdesc;
	  int runNumber;
	  int timeoutOnStop;
	  int monitorSleepSec;
	  int lsTimeOut;
	  int idleRestartTime; 
	
	  unsigned int nbSubProcesses; 
	  int anonymousPipe_[2];
	  bool instance;//?
	  bool hasSharedMemory;
	  bool hasModuleWebRegistry;
	  bool hasServiceWebRegistry;
	  bool haveScalers;
	  unsigned int detachTimeout;
	  log4cplus::LogLevel  logLevel;

	  //sem_t * stop_sem;
	  //int semid;
	  //key_t semkey;
	  sem_t *semaphore;

  };

  //added to the message logger class
  class MasterAppender: public log4cplus::Appender {
    void append(const log4cplus::spi::InternalLoggingEvent &event) {
      std::cout <<"MASTER:"<< event.getMessage() <<std::endl;
    }
    void close(){};

  };

  class SlaveAppender: public log4cplus::Appender {
    void append(const log4cplus::spi::InternalLoggingEvent &event) {
	std::cout <<"SLAVE:"<< event.getMessage() <<std::endl;
    }
    void close(){};

  };

  class FWEPWrapper : public toolbox::lang::Class{

  private:
    //
    edm::EventProcessor             *evtProcessor_;
    bool                             epInitialized_;
    bool                             epCreated_;
    timespec maxWaitTime;

  public:

    FWEPWrapper(log4cplus::Logger &);
    ~FWEPWrapper();
    //virtual ~FWEPWrapper();

    edm::EventProcessor *operator->(){return evtProcessor_;}
    operator bool(){return epInitialized_;}

    // initialize the cmssw event processor
    void init(WrapperConfig & newCfg);
    bool enableSlaveAndWait();
    void killMasterFDs();

  private:
    // stop the cmssw event processor
    edm::EventProcessor::StatusCode stop();

    void attachDqmToShm()   throw (dqmevf::Exception);
    void detachDqmFromShm() throw (dqmevf::Exception);

    // separate threads for communication with the master process
    void startMonitorReceiverWorkLoop() throw (dqmevf::Exception);
    bool monitorReceiver(toolbox::task::WorkLoop* wl);
    bool monitorReceiverAux(toolbox::task::WorkLoop* wl);

    void startStopListenerWorkLoop() throw (dqmevf::Exception);
    bool stopListener(toolbox::task::WorkLoop* w);

    void startScalersWorkLoop() throw (dqmevf::Exception);
    bool scalers(toolbox::task::WorkLoop* wl);

    //master IPC callbacks
    void moduleWeb(xgi::Input *in,xgi::Output *out, std::string);
    void serviceWeb(xgi::Input *in,xgi::Output *out, std::string);
    void microState(xgi::Input *in,xgi::Output *out);
    void defaultWeb(xgi::Input *in,xgi::Output *out);
    void moduleDescs(xgi::Input *in, xgi::Output *out);  
    
    // trigger report callback
    bool getTriggerReport();
    
    //actually enables edm::EP
    bool enableSlave();
  
    //
    // Class members
    //
    WrapperConfig cfg_;
    //tools controlling the input source
    evfSourceVars                   evfSourceVars_;
    dqmevf::InputControllerRegistry *inputControllerReg_;
    dqmevf::MSService               *msService_;

    edm::ServiceToken                serviceToken_;
    //edm::ServiceToken                slaveServiceToken_;
    
    //eventfilter logger
    log4cplus::Logger                log_;

    // infospace (for module and service web registry
    xdata::InfoSpace                *applicationInfoSpace_;

    // monitor infospace
    //->disabled in dqm processor, please ask if you need this

    //ep cfg info retrieved at EP runtime
    std::vector<edm::ModuleDescription const*> descs_;
    std::vector<std::string>           modNames_;
    std::map<std::string,int>           modNameMap_;
    std::vector<std::string>           statNames_;

    // workloop / action signatures in child process
    toolbox::task::WorkLoop         *wlStopListener_;
    toolbox::task::ActionSignature  * asStopListener_;
    bool                            wlStopListenerActive_;
		        
    toolbox::task::WorkLoop         *wlScalers_;
    toolbox::task::ActionSignature  *asScalers_;
    bool                            wlScalersActive_;

    toolbox::task::WorkLoop         *wlMonitorReceiver_;
    toolbox::task::ActionSignature  *asMonitorReceiver_;
    bool                             wlMonitorReceiverActive_;

    toolbox::task::WorkLoop         *wlMonitorReceiverAux_;
    toolbox::task::ActionSignature  *asMonitorReceiverAux_;
    bool                             wlMonitorReceiverActiveAux_;

    //trigger report stuff
    edm::TriggerReport               edmTr_;
    TriggerReportHelpers             trh_;
    std::vector<PathSummaryStatic>   pathStat_;
    std::vector<LSInfo>              lumiReportHistory_;

    //locking objects
    pthread_mutex_t                  ep_guard_lock_;
    pthread_mutex_t                  stop_lock_;
    pthread_mutex_t                  readout_lock_;
    pthread_cond_t                   cond_;

    //locking objects for stopping
    pthread_mutex_t                  stop_thread_blocker_;
    pthread_cond_t                   cond_stop_;

    //switches
    bool                             scalersFirstRun_;   
    bool                             stopCalled_;
    
    std::string                      errorLog_;
    //few helper functions (used only by FWEPWrapper)
  
    void addService(std::vector<edm::ParameterSet>& adjust,std::string const& service);
    void removeServiceMaybe(std::vector<edm::ParameterSet>& adjust,std::string const& service);
    // Add a service to the services list if it is not already there
    void addServiceMaybe(std::vector<edm::ParameterSet>& adjust,std::string const& service);
    const edm::ParameterSet *findService(std::vector<edm::ParameterSet> &adjust, std::string const& service);    
  };

  //web

namespace internal {

  using namespace std;    

  class MyCgi : public xgi::Input{
    public:
      MyCgi() : xgi::Input("",0){}
      std::map<std::string, std::string, std::less<std::string> > &getEnvironment(){return environment_;}
  };

}//internal

}//dqmevf
#endif
