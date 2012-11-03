//% dqmevf::FWEPWrapper class 
//  based on evf::FUEventProcessor implementation
//  located in EventFilter/Processor and EventFilter space

//Author: Srecko Morovic <Srecko.Morovic@cern.ch> 
//and CMS DAQ developers (the original implementation)

#include "FWEPWrapper.h"
//
// ---------------- Instances edm::EventFilter in the child process of dqmevf::Processor -----

#include "procUtils.h"
//remaining evf dependencies
#include "EventFilter/Message2log4cplus/interface/MLlog4cplus.h"
#include "EventFilter/Utilities/interface/ServiceWebRegistry.h"
#include "EventFilter/Utilities/interface/ServiceWeb.h"
#include "EventFilter/Utilities/interface/TimeProfilerService.h"
#include "EventFilter/Utilities/interface/ModuleWebRegistry.h"
#include "EventFilter/Utilities/interface/ModuleWeb.h"
#include "EventFilter/Modules/interface/FUShmDQMOutputService.h"

//edm
#include "FWCore/PluginManager/interface/PresenceFactory.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//dqm
#include "DQMServices/Core/interface/DQMStore.h"

//xdaq
#include "toolbox/task/WorkLoopFactory.h"

#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/FormEntry.h"

//std and boost
#include <vector>
#include <boost/tokenizer.hpp>

//fd removal stuff
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <stdlib.h>
#include <unistd.h>

using namespace cgicc;
using namespace std;

namespace dqmevf{

  //initialized at parent class construction
  FWEPWrapper::FWEPWrapper(log4cplus::Logger &log)
    : evtProcessor_(0)
    , epInitialized_(false)
    , epCreated_(false)
    , inputControllerReg_(0)
    , msService_(0)
    , serviceToken_()
    , log_(log)
    , applicationInfoSpace_(0)

    , wlStopListener_(0)
    , asStopListener_(0)
    , wlStopListenerActive_(false) 
    
    , wlScalers_(0)
    , asScalers_(0)
    , wlScalersActive_(false)

    , wlMonitorReceiver_(0)
    , asMonitorReceiver_(0)
    , wlMonitorReceiverActive_(false)
    
    , wlMonitorReceiverAux_(0)
    , asMonitorReceiverAux_(0)
    , wlMonitorReceiverActiveAux_(false)

    , scalersFirstRun_(true)
    , stopCalled_(false)

  {
  }

  FWEPWrapper::~FWEPWrapper() {evtProcessor_=0;}

  //init and shutdown section ---------------------------------------------------------
  void FWEPWrapper::init(WrapperConfig & newCfg)
  {
    using namespace std;
  
   if (newCfg.idleRestartTime!=0) {
     clock_gettime(CLOCK_REALTIME, &maxWaitTime);
     maxWaitTime.tv_sec += newCfg.idleRestartTime;
   }
   else maxWaitTime.tv_sec=0;

    //init locking objects
    int retval;
    retval = pthread_mutex_init(&readout_lock_,0);
    if(retval != 0) perror("error");
    retval = pthread_mutex_init(&stop_lock_,0);
    if(retval != 0) perror("error");
    retval = pthread_mutex_init(&ep_guard_lock_,0);    
    if(retval != 0) perror("error");
    retval = pthread_cond_init(&cond_,0);
    if(retval != 0) perror("error");

    //stopping controls
    retval = pthread_mutex_init(&stop_thread_blocker_,0);
    if(retval != 0) perror("error");
    retval = pthread_cond_init(&cond_stop_,0);
    if(retval != 0) perror("error");

    //initialize internal fwep configuration copy
    cfg_=newCfg;

    /*
     * Start Workloops
    */
    startStopListenerWorkLoop();
    startMonitorReceiverWorkLoop();

    //setup local logging
    log_.removeAllAppenders ();
    #ifdef DEBUG_EP
    SharedAppenderPtr slaveAppenderPtr = SharedAppenderPtr(new SlaveAppender);
    log_.addAppender(slaveAppenderPtr);
    log_.setLogLevel(cfg_.logLevel);
    #endif

    LOG4CPLUS_INFO(log_,"Initialize CMSSW EventProcessor.");
    LOG4CPLUS_INFO(log_,"CMSSW_BASE:"<<getenv("CMSSW_BASE"));

    //plugin manager init
    edm::AssertHandler ah;
    try{
      //std::cout << "Trying to create message service presence\n";
      edm::PresenceFactory *pf = edm::PresenceFactory::get();
      if(pf != 0) {
        pf->makePresence("MessageServicePresence").release();
      }
      else {
        std::cout << "SLAVE: Unable to create message service presence "<<endl;
      }
    } 
    catch(...) {
      std::cout <<"SLAVE: Unknown Exception (Message Presence)"<<endl;
    }
    
    //service preparation
    boost::shared_ptr<std::vector<edm::ParameterSet> > pServiceSets;
    pServiceSets = cfg_.pdesc->getServicesPSets();

    if(!cfg_.hasSharedMemory){//should be there in DQM
      //addServiceMaybe(*pServiceSets,"DQMStore");
      //addServiceMaybe(*pServiceSets,"DQM");
    }
    else{
      //readd dqm with empty parameters
      //used in case of forwarding dqm data to SM
      removeServiceMaybe(*pServiceSets,"DQMStore");
      removeServiceMaybe(*pServiceSets,"DQM");
      addServiceMaybe(*pServiceSets,"DQMStore");
      addServiceMaybe(*pServiceSets,"DQM");
    }
    //evf
    addServiceMaybe(*pServiceSets,"MLlog4cplus");
    if(cfg_.hasModuleWebRegistry) addServiceMaybe(*pServiceSets,"ModuleWebRegistry");
    else removeServiceMaybe(*pServiceSets,"ModuleWebRegistry");
    if(cfg_.hasServiceWebRegistry) addServiceMaybe(*pServiceSets,"ServiceWebRegistry");
    else removeServiceMaybe(*pServiceSets,"ServiceWebRegistry");
      
    //dqmevf
    addServiceMaybe(*pServiceSets,"MSService");
    if (cfg_.haveScalers) addServiceMaybe(*pServiceSets,"InputControllerRegistry");
    else removeServiceMaybe(*pServiceSets,"InputControllerRegistry");

    //initialize service constructors before EP
    try{
      serviceToken_ = edm::ServiceRegistry::createSet(*pServiceSets);
    }
    catch(cms::Exception &e) {
      cout << "SLAVE: " << e.explainSelf() << endl;
      LOG4CPLUS_ERROR(log_,e.explainSelf());
      exit(EXIT_SUCCESS);
    }    
    catch(std::exception &e) {
      cout << "SLAVE: " << e.what() << endl;
      LOG4CPLUS_ERROR(log_,e.what());
    }
    catch(...) {
      cout << "SLAVE: Unknown Exception" << endl;
      LOG4CPLUS_ERROR(log_,"Unknown Exception");
    }

    //add services which are not constructed before EP
    std::vector<std::string> defaultServices;
    std::vector<std::string> forcedServices;
    
    //add set of usual services
    defaultServices.push_back("MessageLogger");
    defaultServices.push_back("InitRootHandlers");
    defaultServices.push_back("JobReportService");
    
    //finalize service list
    cfg_.pdesc->addServices(defaultServices, forcedServices);

    //Constructing edm::EventProcessor
    try {
      evtProcessor_ = new edm::EventProcessor(cfg_.pdesc,
	  serviceToken_, edm::serviceregistry::kTokenOverrides);
    }
    catch (cms::Exception e) {
      cout << "SLAVE: Exception creating edm:EventProcessor: "<<endl;
      std::cout << "       " << e.explainSelf() << std::endl;
      exit(EXIT_FAILURE);
      //report failed, or shutdown..
    }
    catch (...) {cout << "SLAVE: Unknown Exception"<<endl;}

    pthread_mutex_lock(&readout_lock_); 
    epCreated_=true;
    pthread_mutex_unlock(&readout_lock_); 


    //service setup section
    
    //test rerouting of fwk logging to log4cplus
    edm::LogInfo("FWEPWrapper")<<"started MessageLogger Service.";
    edm::ServiceRegistry::Operate operate(serviceToken_);

    DQMStore *dqm = 0;
    try{
      if(edm::Service<DQMStore>().isAvailable())
	dqm = edm::Service<DQMStore>().operator->();
      //this would kill reference histograms in non-Shm setup
      if(dqm && cfg_.hasSharedMemory) dqm->rmdir("");
    }
    catch(...) {LOG4CPLUS_INFO(log_, "exception when trying to get service DQMStore");}

    evf::ModuleWebRegistry *mwr = 0;
    try{
      if(edm::Service<evf::ModuleWebRegistry>().isAvailable())
	mwr = edm::Service<evf::ModuleWebRegistry>().operator->();
	///if(mwr) mwr->clear(); //private for evf::FWEPWrapper
    }
    catch(...) {LOG4CPLUS_INFO(log_, "exception when trying to get service ModuleWebRegistry");}

    evf::ServiceWebRegistry *swr = 0;
    try{
      if(edm::Service<evf::ServiceWebRegistry>().isAvailable())
	swr = edm::Service<evf::ServiceWebRegistry>().operator->();
    }
    catch(...) {LOG4CPLUS_INFO(log_, "exception when trying to get service ModuleWebRegistry");}


    try{
      if(edm::Service<dqmevf::InputControllerRegistry>().isAvailable())
	inputControllerReg_ = edm::Service<dqmevf::InputControllerRegistry>().operator->();
    }
    catch(...) {LOG4CPLUS_INFO(log_, "exception when trying to get service InputController");}

    try{
      if(edm::Service<dqmevf::MSService>().isAvailable())
	msService_ = edm::Service<dqmevf::MSService>().operator->();
    }
    catch(...) {LOG4CPLUS_INFO(log_, "exception when trying to get service MSService");}
    
    //sending our variables to registered CMSSW classes
    if(mwr) mwr->publish(applicationInfoSpace_);
    if(swr) swr->publish(applicationInfoSpace_);
    
    //start and wait for scalers workloop
    if (cfg_.haveScalers && inputControllerReg_)
    {  
      inputControllerReg_->publish(&evfSourceVars_);
      pthread_mutex_lock(&ep_guard_lock_);
      startScalersWorkLoop();
      cout << "SLAVE: waiting to init scalers workloop..."<<endl;
      pthread_cond_wait(&cond_, &ep_guard_lock_);
      cout << "SLAVE: done"<<endl;
    }  

    pthread_mutex_lock(&readout_lock_); 
    //setup edm ep path name vector
    statNames_.push_back("NotStarted");
    for(unsigned int i = (unsigned int)edm::event_processor::sInit; 
                     i <= (unsigned int)edm::event_processor::sInvalid; i++)
    {
	statNames_.push_back(evtProcessor_->stateName((edm::event_processor::State) i));
    }
    //get module list
    descs_ = evtProcessor_->getAllModuleDescriptions();

    //set up module name vector and map
    modNames_.clear();
    modNames_.push_back("Invalid");
    modNames_.push_back("FWKOVH");
    modNames_.push_back("Input");

    for (unsigned int i=0;i<descs_.size();i++) 
    	modNames_.push_back(descs_[i]->moduleName());
    modNames_.push_back("DQM");
    //map for matching name to integer
    for (unsigned int i=0;i<modNames_.size();i++) modNameMap_[modNames_[i]]=i;

    //setup initial trigger report
    edm::TriggerReport tr;
    evtProcessor_->getTriggerReport(tr);
    trh_.formatReportTable(tr);

    if (evtProcessor_->getState() == edm::event_processor::sError) {
      cout << "SLAVE: FWK EP in Error state after construction and initial setup. Exiting"<<endl;
      exit(EXIT_FAILURE);
    }
    epInitialized_ = true;
    pthread_mutex_unlock(&readout_lock_);
    LOG4CPLUS_INFO(log_," edm::EventProcessor configuration finished.");
    cout << "SLAVE: initialized EP"<<endl;
    return;
  }

  
  //______________________________________________________________________________
  edm::EventProcessor::StatusCode FWEPWrapper::stop()
  {
    //if not initialized, end with success
    cout << "SLAVE: stopping"<<endl;
    stopCalled_=true;
    if (!epInitialized_) return edm::EventProcessor::epOther;

    edm::event_processor::State st = evtProcessor_->getState();
    
    LOG4CPLUS_WARN(log_,"FWEPWrapper::stopEventProcessor. state=" << evtProcessor_->stateName(st));
    
    edm::EventProcessor::StatusCode rc = edm::EventProcessor::epSuccess;
    
    try  {
      if (evtProcessor_->getState()== edm::event_processor::sRunning) {
	if (cfg_.detachTimeout>0) cfg_.timeoutOnStop=cfg_.detachTimeout;
	*evfSourceVars_.runStopFlag=1;
	rc = evtProcessor_->stopAsync(cfg_.timeoutOnStop);
	cout << "SLAVE: stopAsync called"<<endl;
      }
      unsigned int stepMsec=100000;//0.1s
      unsigned int remainingMsec=cfg_.timeoutOnStop*1000000+stepMsec;//1000000!!!
      /*
      while (1) {
        if (remainingMsec<stepMsec) {//timed out
          break;
        }
        typedef  unsigned int uint;
        if ((uint)rc!=(uint)edm::EventProcessor::epSuccess)
        {
          usleep(stepMsec);
          remainingMsec-=stepMsec;
        }
        else break;
        rc = evtProcessor_->statusAsync();
      }
      */	    
      if(rc == edm::EventProcessor::epSuccess) {
	if (remainingMsec>=stepMsec) {
	  cout << "SLAVE: executing endJob"<<endl;
	  evtProcessor_->endJob();
	}
      }
      if(rc == edm::EventProcessor::epTimedOut) {
      }
      else {
	cout << "SLAVE: Processor found in unexpected state"<<endl;
      }
      epInitialized_ = false;
      //} else {
      //todo:notify master?
      //XCEPT_RAISE(dqmevf::Exception,"EventProcessor stop timed out");
            //}
    }
    catch(cms::Exception &e) {
      epInitialized_ = false;
      cout << "SLAVE: exception stopping: " << e.explainSelf() << endl;
      XCEPT_RAISE(dqmevf::Exception,e.explainSelf());
    }    
    catch(std::exception &e) {
      epInitialized_ = false;
      XCEPT_RAISE(dqmevf::Exception,e.what());
    }
    catch(...) {
      epInitialized_ = false;
      XCEPT_RAISE(dqmevf::Exception,"Unknown Exception");
    }
    return rc;
    
  }

  bool FWEPWrapper::getTriggerReport()
  {
    //suspend scalers loop if input controller not present
    if (inputControllerReg_==0) return false;

    edm::TriggerReport tr;
    inputControllerReg_->trapSource("HTTPInputSource",cfg_.lsTimeOut);
    if (stopCalled_) {
	    inputControllerReg_->untrapSource("HTTPInputSource");
	    return false;
    }

    //not yet ready..
    if (evfSourceVars_.lumi==0 || evfSourceVars_.lastLumi==0) {
            inputControllerReg_->untrapSource("HTTPInputSource");
	    return true;
    }
    
    //first invocation (event), do not send report
    if (evfSourceVars_.lastLumi==0) return true;

    unsigned int lastReportedLS_=evfSourceVars_.lumi;

    bool timedOut=evfSourceVars_.hasTimedOut;
    
    if (timedOut) {
        //try again next time
    	inputControllerReg_->untrapSource("HTTPInputSource");
	cout << "SLAVE: INPUT SOURCE LOCK TIMED OUT!"<<endl;
	return true;
    }
    
    //retrieve last received
    LSInfo lastLst;
    if (lumiReportHistory_.size()) {
      lastLst=lumiReportHistory_[lumiReportHistory_.size()-1];
    }

    pthread_mutex_lock(&readout_lock_);
    
    LSInfo newLst;
    newLst.ls = lastReportedLS_-1;//this trigger report was for ls-1
    newLst.proc = evtProcessor_->totalEvents()-lastLst.proc;
    newLst.acc = evtProcessor_->totalEventsPassed()-lastLst.acc;
    if (lumiReportHistory_.size()>100000) //enough for 5000 hours / ~1MB
      lumiReportHistory_.clear();
    else
      lumiReportHistory_.push_back(newLst);

    //get fresh trigger report
    evtProcessor_->getTriggerReport(tr);
    
    pthread_mutex_unlock(&readout_lock_);
   
    inputControllerReg_->untrapSource("HTTPInputSource");
    
    trh_.packTriggerReport(newLst.ls,tr,0);
    return true;
  }


bool FWEPWrapper::enableSlave()
{
  try {    
    evtProcessor_->beginJob();
    cout << "SLAVE: beginJob finished.."<<endl;
    attachDqmToShm();//todo:detach on error
    int sc = 0;
    evtProcessor_->clearCounters();
    evtProcessor_->declareRunNumber(cfg_.runNumber);
    cout << "SLAVE: declared run number "<< cfg_.runNumber << endl;
    ::sleep(1);
    evtProcessor_->runAsync();
    cout << "SLAVE: runAsync executed"<<endl;
    sc = evtProcessor_->statusAsync();
    
    if(sc != 0) {
    std::ostringstream oss;
    oss << "SLAVE: EventProcessor::runAsync returned status code " << sc;
      errorLog_ = oss.str();
      cout << errorLog_ << endl;
      return false;
    }

  }
  catch(cms::Exception &e) {
     errorLog_ = (std::string)e.explainSelf();
     cout << "SLAVE: " << errorLog_ << endl;
     exit(EXIT_FAILURE);
     return false;
  }
  catch(std::exception &e) {
     errorLog_ = (std::string)e.what();
     cout << "SLAVE: " << "std::exception "<< errorLog_ << endl;
     return false;
  }
  catch(...) {
    errorLog_ = "Unknown Exception";
     cout << "SLAVE: " << errorLog_ << endl;
    return false;
  }
  return true;
}


bool FWEPWrapper::enableSlaveAndWait() {

  bool retval=0;
  retval = enableSlave();

  //todo:start workloops before (so that master knows about failures)?
  while(retval && evtProcessor_->getState()!= edm::event_processor::sRunning){
    cout << "SLAVE: waiting for edm::EventProcessor to start before enabling workloops"<<endl;

    if (evtProcessor_->getState()== edm::event_processor::sError) {
      cout << "SLAVE: failure to startup FWK EventProcessor"<<endl;
      exit(EXIT_FAILURE);
    }

    ::sleep(1);
  }
  return retval;
}

void FWEPWrapper::attachDqmToShm() throw (dqmevf::Exception)
{
  if (!cfg_.hasSharedMemory) return;
  std::string errmsg;
  bool success = false;
  try {
    edm::ServiceRegistry::Operate operate(evtProcessor_->getToken());
    if(edm::Service<FUShmDQMOutputService>().isAvailable())
      success = edm::Service<FUShmDQMOutputService>()->attachToShm();
    if (!success) errmsg = "Failed to attach DQM service to shared memory";
  }
  catch (cms::Exception& e) {
    errmsg = "Failed to attach DQM service to shared memory: " + (std::string)e.what();
  }
  if (!errmsg.empty()) XCEPT_RAISE(dqmevf::Exception,errmsg);
}


void FWEPWrapper::detachDqmFromShm() throw (dqmevf::Exception)
{
  if (!cfg_.hasSharedMemory) return;
  std::string errmsg;
  bool success = false;
  try {
    edm::ServiceRegistry::Operate operate(evtProcessor_->getToken());
    if(edm::Service<FUShmDQMOutputService>().isAvailable())
      success = edm::Service<FUShmDQMOutputService>()->detachFromShm();
    if (!success) errmsg = "Failed to detach DQM service from shared memory";
  }
  catch (cms::Exception& e) {
    errmsg = "Failed to detach DQM service from shared memory: " + (std::string)e.what();
  }
  if (!errmsg.empty()) XCEPT_RAISE(dqmevf::Exception,errmsg);
}



// Web callback section --------------------------------------------------------------
  
  //printout module descriptions to master process
  void FWEPWrapper::moduleDescs(xgi::Input *in, xgi::Output *out) {
     if (!descs_.size() || !epInitialized_) return;

     evf::ModuleWebRegistry *mwr = 0;
     edm::ServiceRegistry::Operate operate(evtProcessor_->getToken());
     try {
       if(edm::Service<evf::ModuleWebRegistry>().isAvailable())
         mwr = edm::Service<evf::ModuleWebRegistry>().operator->();
     }
     catch(...) {
       LOG4CPLUS_WARN(log_,"Exception when trying to get service ModuleWebRegistry");
     }

     *out << "<table frame=\"void\" rules=\"rows\" class=\"modules\">"   << std::endl;
     *out << "  <tr>"                                                    << std::endl;
     *out << "    <th colspan=3>"                                        << std::endl;
     *out << "      "                                                    << std::endl;
     if(descs_.size()>0)
       *out << " Process " << descs_[0]->processName()                   << std::endl;
     *out << "    </th>"                                                 << std::endl;
     *out << "  </tr>"                                                   << std::endl;
 
     *out << "  <tr >"                                                   << std::endl;
     *out << "    <th >"                                                 << std::endl;
     *out << "       Module"                                             << std::endl;
     *out << "    </th>"                                                 << std::endl;
     *out << "    <th >"                                                 << std::endl;
     *out << "       Label"                                              << std::endl;
     *out << "    </th>"                                                 << std::endl;
     *out << "    <th >"                                                 << std::endl;
     *out << "       Version"                                            << std::endl;
     *out << "    </th>"                                                 << std::endl;
     *out << "  </tr>"                                                   << std::endl;

   for(unsigned int idesc = 0; idesc < descs_.size(); idesc++)
       {    
           *out << "  <tr>"                                              << std::endl;
         *out << "    <td >";
         if(mwr && mwr->checkWeb(descs_[idesc]->moduleName()))//get from registered module
           *out << "<a href=\"/" << cfg_.urn
                << "module="
                << descs_[idesc]->moduleName() << "\">"
                << descs_[idesc]->moduleName() << "</a>";
         else
           *out << descs_[idesc]->moduleName();

         *out << "</td>"                                                 << std::endl;
         *out << "    <td >";
         *out << descs_[idesc]->moduleLabel();
         *out << "</td>"                                                 << std::endl;
         *out << "    <td >";
         *out << descs_[idesc]->releaseVersion();
         *out << "</td>"                                                 << std::endl;
	  *out << "  </tr>" << std::endl;
	 
    }
    *out << "</table>" << std::endl;
  }
  
  //______________________________________________________________________________
  void FWEPWrapper::moduleWeb(xgi::Input  *in, xgi::Output *out, std::string module)
  {
    using namespace cgicc;
    Cgicc cgi(in);
    std::vector<FormEntry> el1;
    cgi.getElement("module",el1);
    if(evtProcessor_)  {
      //if(el1.size()!=0) {
	//std::string mod = el1[0].getValue();
	edm::ServiceRegistry::Operate operate(evtProcessor_->getToken());
	evf::ModuleWebRegistry *mwr = 0;
	try{
	  if(edm::Service<evf::ModuleWebRegistry>().isAvailable())
	    mwr = edm::Service<evf::ModuleWebRegistry>().operator->();
	}
	catch(...) { 
	  //LOG4CPLUS_WARN(log_, "Exception when trying to get service ModuleWebRegistry");
	}
	if (mwr)
	  mwr->invoke(in,out,module);
	else *out << "ModuleWebRegistry not present " << std::endl;
      //}
    }
    else {
      *out<<"EventProcessor just disappeared "<<std::endl;
    }
  }
  

  //______________________________________________________________________________
  void FWEPWrapper::serviceWeb(xgi::Input  *in, xgi::Output *out, std::string service)
  {
    //using namespace cgicc;
    //Cgicc cgi(in);
    //std::vector<FormEntry> el1;
    //cgi.getElement("service",el1);
    if(evtProcessor_)  {
      //if(el1.size()!=0) {
	//std::string ser = el1[0].getValue();
	edm::ServiceRegistry::Operate operate(evtProcessor_->getToken());
	evf::ServiceWebRegistry *swr = 0;
	try{
	  if(edm::Service<evf::ServiceWebRegistry>().isAvailable())
	    swr = edm::Service<evf::ServiceWebRegistry>().operator->();
	}
	catch(...) { 
	  //LOG4CPLUS_WARN(log_, "Exception when trying to get service ModuleWebRegistry");
	}
	if (swr)
	  swr->invoke(in,out,service);
	else *out << "ServiceWebRegistry not present " << std::endl;
      //}
    }
    else {
      *out<<"EventProcessor just disappeared "<<std::endl;
    }
  }

//______________________________________________________________________________
//child state
void FWEPWrapper::microState(xgi::Input  *in, xgi::Output *out)
{

    std::string micro1 = "Unavailable";
    std::string micro2 = "Unavailable";
    
    if(epInitialized_)
      micro1 = "Initialized";

    if (evtProcessor_!=0) micro1 = evtProcessor_->currentStateName();
      else micro1=statNames_[0]; //should not happen    
    
    MSService * msHandle = msService_;
    if(msService_) {
      if (!stopCalled_)
        micro2 = msHandle->getMicroState2();
      else micro2 = "END";
    }
    else if (epInitialized_) 
    {
      micro2= "Initialized (no mss)";
    }
    
    *out << "<td>" << micro1 << "</td>";
    *out << "<td>" << micro2 << "</td>";
    *out << "<td>" << evfSourceVars_.evtLumi << "/" << evfSourceVars_.hasTimedOut << "</td>";
}
  
void FWEPWrapper::defaultWeb(xgi::Input  *in, xgi::Output *out)
{

  *out << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
       << "<html><head><title>" <<  " THIS IS A CHILD PROCESS!!! slot:" << cfg_.slot << "</title>"
       //<< "<meta http-equiv=\"REFRESH\" content=\"0;url=/evf/html/defaultBasePage.html\">"
       << "</head></html>";
}


//Workloops section (child process threads)_____________________________________

//workloop that listens exclusively to stop message
//______________________________________________________________________________
void FWEPWrapper::startStopListenerWorkLoop() throw (dqmevf::Exception)
{
  try {
    wlStopListener_ = toolbox::task::getWorkLoopFactory()->getWorkLoop("StopListener", "waiting");
    if (!wlStopListener_->isActive()) wlStopListener_->activate();
    asStopListener_ = toolbox::task::bind(this,&FWEPWrapper::stopListener, "StopListener");
    wlStopListener_->submit(asStopListener_);
    wlStopListenerActive_ = true;
  }
  catch (xcept::Exception& e) {
    std::string msg = "Failed to start workloop 'StopListener'.";
    XCEPT_RETHROW(dqmevf::Exception,msg,e);
  }
}

bool FWEPWrapper::stopListener(toolbox::task::WorkLoop *)
{
  MsgBuf msgRec(0, MSQM_MESSAGE_TYPE_STOP);
  try{
    if (cfg_.semaphore) sem_wait(cfg_.semaphore);
    usleep(1000);
    {
      pthread_mutex_lock(&stop_lock_);
      try {
	LOG4CPLUS_INFO(log_,"Start stopping :) ...");

	//skipping EP stop in nbSubs>1
	if (cfg_.nbSubProcesses<2) {

	  edm::EventProcessor::StatusCode rc = stop();

	  std::stringstream ssout;

	  if (rc==edm::EventProcessor::epSuccess) {
	    ssout << "SLAVE: EventProcessor exit successful";
	    cout << "SLAVE: EventProcessor exit successful" << std::endl;

	    errorLog_ = ssout.str();
	  }
	  else {
	    cout << "SLAVE: STOP:EventProcessor killed or exited with code: " << rc << endl;
	    //localLog(errorLog_);
	  }
	}
      }
      catch (xcept::Exception &e) {

	cout << "SLAVE: STOP:Exception: " << (std::string)e.what() << endl;
	std::stringstream ssout;
	ssout << "SLAVE: stopping FAILED: " << (std::string)e.what();
	errorLog_= ssout.str();
	//localLog(errorLog_);
      }
      catch (...) { cout << "SLAVE: STOP:Unknown Exception"<<endl;}

      LOG4CPLUS_INFO(log_,"SLAVE: Finished stopping!");

      usleep(10000);//10ms
      MsgBuf msg1(0,MSQS_MESSAGE_TYPE_STOP);
      if (cfg_.detachTimeout==0) //avoid reply if in detached mode
        cfg_.sub->postSlaveSup(msg1);//reply
      //usleep(100000);//wait 0.1s
      cout << "CHILD PROCESS EXIT\n";
      fclose(stdout);
      fclose(stderr);
      //pthread_mutex_unlock(&stop_lock_);
      exit(EXIT_SUCCESS);
    }
  }
  catch(dqmevf::Exception &e){cout << "SLAVE: stopping msg queue not intialized?"<<endl;}
  sleep(5);//spin protection
  return true;
}

void FWEPWrapper::startMonitorReceiverWorkLoop() throw (dqmevf::Exception)
{
  try {
    wlMonitorReceiver_=
      toolbox::task::getWorkLoopFactory()->getWorkLoop("MonitorReceiver","waiting");
    if (!wlMonitorReceiver_->isActive()) 
      wlMonitorReceiver_->activate();
    asMonitorReceiver_ = toolbox::task::bind(this,&FWEPWrapper::monitorReceiver, "MonitorReceiver");
    wlMonitorReceiver_->submit(asMonitorReceiver_);
    wlMonitorReceiverActive_ = true;
  }
  catch (xcept::Exception& e) {
    std::string msg = "Failed to start workloop 'MonitorReceiver'.";
    XCEPT_RETHROW(dqmevf::Exception,msg,e);
  }
  
  //start another workloop to receive from supervisor state queries
  try {
    wlMonitorReceiverAux_=
      toolbox::task::getWorkLoopFactory()->getWorkLoop("MonitorReceiverAux","waiting");
    if (!wlMonitorReceiverAux_->isActive()) 
      wlMonitorReceiverAux_->activate();
    asMonitorReceiverAux_ = toolbox::task::bind(this,&FWEPWrapper::monitorReceiverAux, "MonitorReceiverAyx");
    wlMonitorReceiverAux_->submit(asMonitorReceiverAux_);
    wlMonitorReceiverActiveAux_ = true;
  }
  catch (xcept::Exception& e) {
    std::string msg = "Failed to start workloop 'MonitorReceiver'.";
    XCEPT_RETHROW(dqmevf::Exception,msg,e);
  }

}

bool FWEPWrapper::monitorReceiverAux(toolbox::task::WorkLoop *)
{
  MsgBuf msg(0,MSQM_MESSAGE_TYPE_PRG);
  try{
    //define variables
    MsgBuf msg1(sizeof(prg),MSQS_MESSAGE_TYPE_PRR);
    prg *data;
    MSService * msHandle;

    unsigned int ret = cfg_.sub->rcvSlaveSup(msg); //receive messages from Master
    switch(ret) {
      case MSQM_MESSAGE_TYPE_PRG:
	data           = (prg*)msg1->mtext;
	pthread_mutex_lock(&readout_lock_);
	if (epInitialized_) {
	  data->ls             = evfSourceVars_.evtLumi;
	  data->nbp            = evtProcessor_->totalEvents();
	  data->nba            = evtProcessor_->totalEventsPassed();
	  data->Ms             = ((int) evtProcessor_->getState()) + 1;
	  //get module state
	  msHandle = msService_;
	  if(msService_) {
	    data->ms    = modNameMap_[msHandle->getMicroState2()]+1;
	    //std::cout << msHandle->getMicroState2() << endl;//DEBUG
	  }
	  else { data->ms = 0;}
	} else
	{
	  data->ls = 0;
	  data->nbp = 0;
	  data->nba = 0;
	  data->Ms  = 0;
	  data->ms  = 0;
	}
	data->dqm=0;

	//exit check
	if(epCreated_) {
	  if (evtProcessor_->getState() == edm::event_processor::sError) 
	  {
	    pthread_mutex_lock(&stop_lock_);
	    cout << "SLAVE: Processor found in error state - will exit..."<<endl;
	    sleep(1);
	    exit(EXIT_FAILURE);
	    /* no need to unlock mutex after exit :-)*/
	  }
	}
	//timeout exit check if not processed at least one event
	if (maxWaitTime.tv_sec!=0 && data->nbp==0) {
	  timespec tsnew;
	  clock_gettime(CLOCK_REALTIME, &tsnew);
	  if (tsnew.tv_sec > maxWaitTime.tv_sec) {
	    cout << "SLAVE: event not processed for configured time of " << cfg_.idleRestartTime <<". Exiting"<< endl; 
	    exit(EXIT_FAILURE);
	  }
	}
	pthread_mutex_unlock(&readout_lock_);

	if (!(stopCalled_ && cfg_.detachTimeout>0))
	  cfg_.sub->postSlaveSup(msg1);
	usleep(100000);
	break;
      case MSQM_MESSAGE_TYPE_STOP:
	pthread_cond_signal(&cond_stop_);
	break;
      default:
	cout << "SLAVE: UNHANDLED MSG IN AUX LOOP "<<ret<<endl;
    }
  }
  catch(dqmevf::Exception &e){
	  std::cout << "SLAVE: exception caught in recevingMAux: " << e.what() << endl;
	  sleep(1);
	  pthread_mutex_unlock(&readout_lock_);
  }
  if (stopCalled_ && cfg_.detachTimeout>0) return false;
  return true;
}

bool FWEPWrapper::monitorReceiver(toolbox::task::WorkLoop *)
{
  MsgBuf msg;
  try{
    unsigned int ret = cfg_.sub->rcvSlaveMon(msg); //will receive only messages from Master
    if (stopCalled_ && cfg_.detachTimeout>0) return false;
    switch(ret)
      {
      case MSQM_MESSAGE_TYPE_MCS:
	{
	  xgi::Input *in = 0;
	  xgi::Output out;
	  pthread_mutex_lock(&readout_lock_);
	  microState(in,&out);
	  pthread_mutex_unlock(&readout_lock_);
	  MsgBuf msg1(out.str().size(),MSQS_MESSAGE_TYPE_MCR);
	  strncpy(msg1->mtext,out.str().c_str(),out.str().size());
	  cfg_.sub->postSlaveMon(msg1);
	  break;
	}
      case MSQM_MESSAGE_TYPE_WEB:
	{
	  xgi::Input  *in = 0;
	  xgi::Output out;
	  unsigned int bytesToSend = 0;
	  MsgBuf msg1(NUMERIC_MESSAGE_SIZE,MSQS_MESSAGE_TYPE_WEB);
	  std::string query = msg->mtext;
	  size_t pos = query.find_first_of("&");
	  std::string method;
	  std::string args;
	  if(pos!=std::string::npos)  
	    {
	      method = query.substr(0,pos);
	      args = query.substr(pos+1,query.length()-pos-1);
	    }
	  else
	    method=query;
	  
	  pthread_mutex_lock(&readout_lock_);
	  
	  if (method=="epStateIndex") {
	  	for (unsigned int i=0;i<statNames_.size();i++) {
		  out << statNames_[i];
		  if (i<statNames_.size()-1) out << ";";
		}
	  }
	  else if (method=="epModuleIndex") {
	  	for (unsigned int i=0;i<modNames_.size();i++) {
		  out << modNames_[i];
		  if (i<modNames_.size()-1) out << ";";
		}
	  }

	  else if (method=="epModuleDescs") {
	      moduleDescs(in,&out);
	  }
	  else if(method=="procStat")
	    {
	      utils::procStat(&out);
	    }
	  else if(method=="moduleWeb" || method=="serviceWeb")
	    {
	      internal::MyCgi mycgi;
	      boost::char_separator<char> sep(";");
	      boost::tokenizer<boost::char_separator<char> > tokens(args, sep);
	      for (boost::tokenizer<boost::char_separator<char> >::iterator tok_iter = tokens.begin();
		   tok_iter != tokens.end(); ++tok_iter){
		size_t pos = (*tok_iter).find_first_of("%");
		if(pos != std::string::npos){
		  std::string first  = (*tok_iter).substr(0    ,                        pos);
		  std::string second = (*tok_iter).substr(pos+1, (*tok_iter).length()-pos-1);
		  mycgi.getEnvironment()[first]=second;
		}
	      }
	      //extract module/service parameter for the method
	      Cgicc cgi(in);
	      std::vector<FormEntry> el1;

	      if (method=="moduleWeb") {
		cgi.getElement("module",el1);
		std::string modName;
		if(el1.size()!=0) modName = el1[0].getValue();
		moduleWeb(&mycgi,&out,modName);
	      }
	      if (method=="serviceWeb") {
		cgi.getElement("service",el1);
		std::string srvName;
		if(el1.size()!=0) srvName = el1[0].getValue();
		serviceWeb(&mycgi,&out,srvName);
	      }
	    }
	  else if(method=="Default")
	    {
	      defaultWeb(in,&out);
	    }
	  else 
	    {
	      out << "Error 404!" << std::endl;
	    }
          pthread_mutex_unlock(&readout_lock_);
	  if (!out.str().size()) bytesToSend=0;
	  else
	    bytesToSend = out.str().size()+1;//0 terminator added
	  unsigned int cycle = 0;
	  if(bytesToSend==0)
	    {
	      snprintf(msg1->mtext, NUMERIC_MESSAGE_SIZE, "%d", bytesToSend);
	      cfg_.sub->postSlaveMon(msg1);
	    }
	  while(bytesToSend !=0){
	    unsigned int msgSize = bytesToSend>MAX_PIPE_BUFFER_SIZE ? MAX_PIPE_BUFFER_SIZE : bytesToSend;
	    snprintf(msg1->mtext, NUMERIC_MESSAGE_SIZE, "%d", msgSize);
	    write(cfg_.anonymousPipe_[PIPE_WRITE],
		  out.str().c_str()+MAX_PIPE_BUFFER_SIZE*cycle,
		  msgSize);
	    cfg_.sub->postSlaveMon(msg1);
	    bytesToSend -= msgSize;
	    cycle++;
	  }
	  break;
	}
      case MSQM_MESSAGE_TYPE_TRP:
	{
	  break;
	}
      }
  }
  catch(dqmevf::Exception &e){
    cout << "SLAVE: exception caught in recevingM: " << e.what() << endl;
    if (stopCalled_ && cfg_.detachTimeout>0) return false;
    sleep(1);
  }
  return true;
}


//todo:check
//______________________________________________________________________________
void FWEPWrapper::startScalersWorkLoop() throw (dqmevf::Exception)
{
  try {
    wlScalers_=
      toolbox::task::getWorkLoopFactory()->getWorkLoop("Scalers", "waiting");
    if (!wlScalers_->isActive()) wlScalers_->activate();
    asScalers_ = toolbox::task::bind(this,&FWEPWrapper::scalers, "Scalers");
    
  wlScalers_->submit(asScalers_);
  wlScalersActive_ = true;
  }
  catch (xcept::Exception& e) {
    std::string msg = "Failed to start workloop 'Scalers'.";
    XCEPT_RETHROW(dqmevf::Exception,msg,e);
  }
}

bool FWEPWrapper::scalers(toolbox::task::WorkLoop* wl)
{
  
  if(evtProcessor_)
    {
      //return if invoked before ep setup
      if (scalersFirstRun_) {
	pthread_mutex_lock(&ep_guard_lock_);
	scalersFirstRun_=false;
	if (inputControllerReg_) inputControllerReg_->untrapSource("HTTPInputSource");
	//let the init continue 
        pthread_cond_signal(&cond_);
	pthread_mutex_unlock(&ep_guard_lock_);
      }
      if (!epInitialized_) {sleep(2);return true;}
      if (!getTriggerReport()) {
        //stop called
	wlScalersActive_ = false;
	return false;
      }
    }
  else
    {
      cout << getpid()<< "SLAVE:  Error: scalers called on edm::EventProcessor uninitialized " << endl;
      pthread_mutex_lock(&ep_guard_lock_);
      pthread_cond_signal(&cond_);
      pthread_mutex_unlock(&ep_guard_lock_);
      wlScalersActive_ = false;
      return false;
    }
  //pthread_mutex_lock(&readout_lock_);
  int ret=0;
  if (!stopCalled_)
    ret = cfg_.sub->postSlaveSup(trh_.getPackedTriggerReport());
  //pthread_mutex_unlock(&readout_lock_);
  if(ret!=0) 
    cout << "SLAVE: scalers workloop, error posting to sqs_ " << errno << endl;

  usleep(100000);//spin protection (sleep 0.1s)
  return true;
}

//utilities

void FWEPWrapper::addService(std::vector<edm::ParameterSet>& adjust,std::string const& service)
    {
      edm::ParameterSet newpset;
      newpset.addParameter<std::string>("@service_type",service);
      adjust.push_back(newpset);
    }

void FWEPWrapper::removeServiceMaybe(std::vector<edm::ParameterSet>& adjust,std::string const& service)
    {
      std::vector<edm::ParameterSet>::iterator it;
      for(it=adjust.begin();it!=adjust.end();++it) {
	std::string name = it->getParameter<std::string>("@service_type");
	if (name == service) break;
      }
      if(it!=adjust.end()) adjust.erase(it);
    }

    // Add a service to the services list if it is not already there
void FWEPWrapper::addServiceMaybe(std::vector<edm::ParameterSet>& adjust,std::string const& service)
    {
      std::vector<edm::ParameterSet>::const_iterator it;
      for(it=adjust.begin();it!=adjust.end();++it) {
	std::string name = it->getParameter<std::string>("@service_type");
	if (name == service) return;
      }
      addService(adjust, service);
    }
    
const edm::ParameterSet* FWEPWrapper::findService(std::vector<edm::ParameterSet> &adjust, std::string const& service)
    {
      edm::ParameterSet *retval = 0;
      std::vector<edm::ParameterSet>::const_iterator it;
      for(it=adjust.begin();it!=adjust.end();++it) {
	std::string name = it->getParameter<std::string>("@service_type");
	if (name == service) return &(*it);
      }
      return retval;
    }

void FWEPWrapper::killMasterFDs(){

  //find all socket file descriptors inherited from parent process and close them
  //code Borrowed from the EventFilter/Processor

  pid_t pid = ::getpid();
  std::ostringstream ost;
  ost << "/proc/" << pid << "/fd/";
  DIR *dir = opendir(ost.str().c_str());
  dirent *de;
  struct stat buf;
  std::vector<int> oldfds;
  std::vector<int> newfds;

  while((de = readdir(dir))!=0){
    char *name = de->d_name;
    std::string path = ost.str()+name;
    stat(path.c_str(),&buf);
    if(S_ISSOCK(buf.st_mode)){
      int fd = atoi(name);
      oldfds.push_back(fd);
      int newfd = dup(fd);
      if(newfd>0) newfds.push_back(newfd);
      else std::cout <<"couldn't duplicate old fd " << fd << std::endl;
    }
  }
  closedir(dir);
  for(unsigned int i = 0; i < oldfds.size(); i++){
    close(oldfds[i]);
    int newfd = dup2(newfds[i],oldfds[i]);
    if(newfd!=oldfds[i]) std::cout <<"couldn't duplicate new fd to old "
                                   << oldfds[i] << std::endl;
    close(newfds[i]);
  }
}

}//namespace dqmevf
