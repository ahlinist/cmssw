//% dqmevf::Processor class 
//  based on evf::FUEventProcessor implementation
//  located in EventFilter/Processor and EventFilter space

//Author: Srecko Morovic <Srecko.Morovic@cern.ch> 
//and CMS DAQ developers (the original implementation)

#include "Processor.h"
//
// ---------------- Xdaq Application controlling the dqmevf::FWEPWrapper through IPC -----


#include "ParameterSetRetriever.h"
#include "procUtils.h"

#include "FWCore/PluginManager/interface/ProblemTracker.h"
#include "FWCore/PluginManager/interface/PresenceFactory.h"
#include "FWCore/Utilities/interface/Presence.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Version/interface/GetReleaseVersion.h"

#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"

#include "EventFilter/Message2log4cplus/interface/MLlog4cplus.h"

#include "pt/PeerTransportReceiver.h"
#include "pt/PeerTransportAgent.h"

#include "xcept/tools.h"
#include "xgi/Method.h"

#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/FormEntry.h"

#include <sys/wait.h>
#include <sys/utsname.h>

#include <typeinfo>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

#include <boost/tokenizer.hpp>

#include <sys/mman.h>

//#define DEBUG_EP 1

using namespace dqmevf;
using namespace cgicc;
using namespace std;


//______________________________________________________________________________
Processor::Processor(xdaq::ApplicationStub *s) 
  : xdaq::Application(s)
  
  , applicationInfoSpace_(0)
  , fsm_(this)
  , log_(getApplicationLogger())
  , fwepWrapper_(log_)
  , runNumber_(0)
  , runType_("")
  , runKey_("")
  , runTypeAuto_(true)
  , autoRestartSlaves_(false)
  , slaveRestartDelaySecs_(10)
  , hasShMem_(true)
  , hasModuleWebRegistry_(true)
  , hasServiceWebRegistry_(true)
  , exitOnError_(true)
  , superSleepSec_(1)
  , monSleepSec_(1)
  , lsTimeOut_(100000000) //usec
  , idleRestartTime_(0)
  , timeoutOnStop_(10)
  , detachTimeout_(0)
  , msgQueueOffset_(0)

  , reasonForFailedState_()
  , logRing_(logRingSize_)
  , logRingIndex_(logRingSize_)
  , logWrap_(false)
  , nbSubProcesses_(1)
  , nbSubProcessesReporting_(0)
  , lastReceivedTrLs_(0)
  , trhHistoryIndex_(0)
  , trhSumAcc_(0)
  , lastKnownLS_(0)

  , wlSupervising_(0)
  , asSupervisor_(0)
  , supervising_(false)

  , wlSummarize_(0)
  , asSummarize_(0)
  , wlSummarizeActive_(false)

  , hasScalersService_(true)

  //, configurationInitialized_(false)
  , configurationInit_(false)
  , isStopping_(false)
  , inForcedKillMask_(0)
  , debug_(false)
  , degradedCounter_(0)
  , startedCounter_(0)
{
  using namespace utils;
 
  // create pipe for web communication
  int retpipe = pipe(anonymousPipe_);
  
  if(retpipe != 0)  LOG4CPLUS_ERROR(getApplicationLogger(),"Failed to create pipe");

  //pass application parameters to FWEPWrapper
  
  // bind relevant callbacks to finite state machine
  fsm_.initialize<dqmevf::Processor>(this);

  //debug: set local logging
#ifdef DEBUG_EP
  log4cplus::Logger & log_= getApplicationLogger();
  log_.removeAllAppenders ();
  SharedAppenderPtr masterAppenderPtr = SharedAppenderPtr(new MasterAppender);
  log_.addAppender(masterAppenderPtr);
  log_.setLogLevel(DEBUG_LOG_LEVEL);
#endif


  instance_=getApplicationDescriptor()->getInstance();
  LOG4CPLUS_INFO(getApplicationLogger(),"CMSSW_BASE:"<<getenv("CMSSW_BASE"));

  getApplicationDescriptor()->setAttribute("icon", "/evf/images/metal029.jpg");
  wCfg_.xdaqApp=(xdaq::Application*)this;
  
  xdata::InfoSpace *ispace = getApplicationInfoSpace();
  applicationInfoSpace_ = ispace;

  // default configuration
  ispace->fireItemAvailable("rcmsStateListener",     fsm_.rcmsStateListener()     );

  ispace->fireItemAvailable("parameterSet",         &configString_                );
  ispace->fireItemAvailable("parameterSetCopy",     &configStringCopy_            );

  ispace->fireItemAvailable("stateName",             fsm_.stateName()             );
  ispace->fireItemAvailable("runNumber",            &runNumber_                   );
  ispace->fireItemAvailable("runType",              &runType_                     );
  ispace->fireItemAvailable("runKey",               &runKey_                      );
  ispace->fireItemAvailable("runTypeAuto",          &runTypeAuto_                 );

  //todo: autodetect dqm shm output service
  ispace->fireItemAvailable("hasSharedMemory",      &hasShMem_                    );
  ispace->fireItemAvailable("hasModuleWebRegistry", &hasModuleWebRegistry_        );
  ispace->fireItemAvailable("hasServiceWebRegistry", &hasServiceWebRegistry_      );
  ispace->fireItemAvailable("hasInputSourceSync",     &hasScalersService_         );

  //todo: set this offset from FM
  ispace->fireItemAvailable("msgQueueOffset",       &msgQueueOffset_              );
  ispace->fireItemAvailable("nbSubProcesses",       &nbSubProcesses_              );
  ispace->fireItemAvailable("nbSubProcessesReporting",&nbSubProcessesReporting_   );

  ispace->fireItemAvailable("superSleepSec",        &superSleepSec_               );
  ispace->fireItemAvailable("monSleepSec",          &monSleepSec_                 );
  
  ispace->fireItemAvailable("autoRestartSlaves",    &autoRestartSlaves_           );
  ispace->fireItemAvailable("slaveRestartDelaySecs",&slaveRestartDelaySecs_       );
  
  ispace->fireItemAvailable("timeoutOnStop",        &timeoutOnStop_               );
  ispace->fireItemAvailable("detachTimeout",        &detachTimeout_               );
  ispace->fireItemAvailable("lsTimeOutUsec",        &lsTimeOut_                   );
  ispace->fireItemAvailable("idleRestartTime",        &idleRestartTime_           );

  detachedRunning_=false;
  ispace->fireItemAvailable("detachedRunning",      &detachedRunning_             );

  //ispace->fireItemAvailable("logLevel",           &logLevel_			  );

  //configurationInitialized_ = false;
  getApplicationInfoSpace()->addItemRetrieveListener ("parameterSetCopy", this);

  // findRcmsStateListener
  fsm_.findRcmsStateListener();
  
  // Bind web interface
  xgi::bind(this, &Processor::css,              "styles.css");
  xgi::bind(this, &Processor::defaultWebPage,   "Default"   );
  xgi::bind(this, &Processor::spotlightWebPage, "Spotlight" );
  //xgi::bind(this, &Processor::subWeb,           "SubWeb"    );
  xgi::bind(this, &Processor::moduleWeb,        "moduleWeb" );
  xgi::bind(this, &Processor::serviceWeb,       "serviceWeb");
  xgi::bind(this, &Processor::microState,       "microState");
  xgi::bind(this, &Processor::updater,          "updater"   );
  xgi::bind(this, &Processor::procStat,         "procStat"  );
  xgi::bind(this, &Processor::initiateReset, "initiateReset");

  xgi::bind(this, &Processor::manualConfigure, "manualConfigure");
  xgi::bind(this, &Processor::manualEnable, "manualEnable");
  xgi::bind(this, &Processor::manualHalt, "manualHalt");
//  xgi::bind(this, &Processor::manualRecover, &manualRecover);
  // instantiate the plugin manager, not referenced here after!
  
  ML::MLlog4cplus::setAppl(this);

  typedef std::set<xdaq::ApplicationDescriptor*> AppDescSet_t;
  typedef AppDescSet_t::iterator                 AppDescIter_t;

  masterStat_ = masterStatT(); 
  
  pthread_mutex_init(&start_lock_,0);
  pthread_mutex_init(&start_lock2_,0);
  pthread_mutex_init(&stop_lock_,0);
  pthread_mutex_init(&pickup_lock_,0);
  pthread_mutex_init(&cfg_refresh_lock_,0);
  pthread_mutex_init(&sup_queue_lock_,0);
  pthread_mutex_init(&summary_queue_lock_,0);
  
  semaphore_ = 0;
  makeStaticInfo();
  webHelper_ = new Web();
  ostringstream pidstr;
  pidstr << getpid();
  webHelper_->setJCLogURL( getApplicationDescriptor()->getContextDescriptor()->getURL(), pidstr.str() );
}


//______________________________________________________________________________
Processor::~Processor() {}


////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

void Processor::makeStaticInfo()
{
  using namespace utils;
  std::ostringstream ost;
  mDiv(&ost,"ve");
  ost<< "Revision: V01-00-05 (" << edm::getReleaseVersion() <<")";
  cDiv(&ost);

  mDiv(&ost,"cl");
  ost << getApplicationDescriptor()->getClassName();
  cDiv(&ost);
  mDiv(&ost,"in",getApplicationDescriptor()->getInstance());

  //mDiv(&ost,"sh",hasShMem_.toString());
  //mDiv(&ost,"mw",hasModuleWebRegistry_.toString());
  //mDiv(&ost,"sw",hasServiceWebRegistry_.toString());
  //mDiv(&ost,"ms",monSleepSec_.toString());
  //mDiv(&ost,"lst",lsTimeOut_.toString());

  char cbuf[sizeof(struct utsname)];
  struct utsname* buf = (struct utsname*)cbuf;
  uname(buf);
  mDiv(&ost,"sysinfo");
  ost << buf->nodename << "        " << buf->sysname << "   " << buf->release << "   " << buf->machine;
//      << "  " << buf->release << "  " << buf->version << "  " <;
  cDiv(&ost);
  updaterStatic_ = ost.str();
}

bool Processor::testEDMConfiguration() {
  try {
    ParameterSetRetriever pr(configString_);
    PythonProcessDesc ppdesc = PythonProcessDesc(pr.getAsString());
    //ppdesc.processDesc();
  }
  catch (...) {return false;}
  return true;
}

//read cfg for display in master GUI
bool Processor::readEDMConfiguration() {
  try {
    friendlyPythonCfg_ = std::string();
    ParameterSetRetriever pr(configString_,0);
    friendlyPythonCfg_ = pr.getAsString();
  }
  catch (...) {return false;}
  return true;
}

bool Processor::initEDMConfiguration() {

  //if (configurationInitialized_) return true;
  pthread_mutex_lock(&cfg_refresh_lock_);

  //wipe out previous configuration
  friendlyPythonCfg_ = std::string();
  wCfg_.configPathTable = std::string();
  wCfg_.pdesc.reset();
  configStringCopy_=std::string();

  try {
    ParameterSetRetriever pr(configString_,0);

    friendlyPythonCfg_ = pr.getAsString();
    //std::cout << friendlyPythonCfg_ << std::endl;
    wCfg_.configPathTable = pr.getPathTableAsString();

    //add runtype and runkey as "command line" python parameters
    char cmsRunName_[]="cmsRun";
    string argv1="runtype="+runType_.value_;
    string argv2="runkey="+runKey_.value_;
    const char* argvArray[4]={cmsRunName_, configString_.value_.c_str(), argv1.c_str(), argv2.c_str()};
    size_t argcParam=4;
    if (runType_.value_=="") {argvArray[2]=argv2.c_str();argcParam=3;};
    if (runType_.value_=="" && runKey_.value_=="") argcParam=2;

    PythonProcessDesc ppdesc = PythonProcessDesc(friendlyPythonCfg_,argcParam,(char**)argvArray);

    wCfg_.pdesc = ppdesc.processDesc();
    configStringCopy_ = wCfg_.pdesc->dump();
    //configurationInitialized_=true;
  }
  catch (...) 
  {
    //LOG4CPLUS_INFO(getApplicationLogger(),"exception in psetRetriever - file missing or syntax error?\n");
    cout << "exception in psetRetriever - file missing or syntax error?"<<endl;
    //configurationInitialized_=false;
    pthread_mutex_unlock(&cfg_refresh_lock_);
    return false;
  }
  pthread_mutex_unlock(&cfg_refresh_lock_);
  return true;
}

void Processor::spawnChild(unsigned int i) {
  //begin child process stuff (restarting)
  int retval;
  retval = pthread_mutex_destroy(&summary_queue_lock_);//let summarize wl shut down
  if(retval != 0) perror("error");
  retval = pthread_mutex_destroy(&stop_lock_);
  if(retval != 0) perror("error");
  retval = pthread_mutex_destroy(&start_lock_);
  if(retval != 0) perror("error");
  pthread_mutex_destroy(&start_lock2_);
  if(retval != 0) perror("error");
  pthread_mutex_destroy(&pickup_lock_);
  if(retval != 0) perror("error");
  pthread_mutex_destroy(&cfg_refresh_lock_);
  if(retval != 0) perror("error");
  pthread_mutex_destroy(&sup_queue_lock_);
  if(retval != 0) perror("error");

  //delete peer transport for child process
  try{
    std::cout << "CHILD:Destroying the PeerTransport instance"<<std::endl;
    //wait for connection handling to finish? (deadlock possibility if not)
    usleep(100000);
    pt::PeerTransport * ptr =
      pt::getPeerTransportAgent()->getPeerTransport("http","soap",pt::Receiver);
    delete ptr;
  }
  catch (pt::exception::PeerTransportNotFound & e ){
    LOG4CPLUS_WARN(getApplicationLogger()," ***Slave Failed to shutdown ptHTTP");
  }

  wCfg_.runNumber=runNumber_.value_;
  wCfg_.sub=&subs_[i];
  wCfg_.slot = i;

  //init python
  std::cout << "SLAVE:Initializing Python configuration"<<std::endl;
  bool ret = initEDMConfiguration();
  if (!ret) {
    //terminate child process if configuration was not successful
    cout << "SLAVE: Start aborted: Configuration error. "+configString_.value_ << endl;
    exit(EXIT_FAILURE);
    return;
  }

  fwepWrapper_.init(wCfg_);
  fsm_.disableRcmsStateNotification();

  //startup EDM Processor
  bool rets = fwepWrapper_.enableSlaveAndWait();

  //finish parent class transition in child (maybe not required)
  try {
    if (!rets) {
      fsm_.fireFailed("error",this);
      exit(EXIT_FAILURE);
    }
    else {
      fsm_.fireEvent("EnableDone",this);
    }
  }
  catch (xcept::Exception &e)
  {
    std::cout << "exception " << (std::string)e.what() << std::endl;
  }
  std::cout << "SLAVE:Enable Done" << std::endl;
}

//______________________________________________________________________________
xoap::MessageReference Processor::fsmCallback(xoap::MessageReference msg)
  throw (xoap::exception::Exception)
{
  return fsm_.commandCallback(msg);
}

//______________________________________________________________________________
void Processor::actionPerformed(xdata::Event& e)
{
  if (e.type()=="ItemRetrieveEvent") {
    std::string item = dynamic_cast<xdata::ItemRetrieveEvent&>(e).itemName();
    if (item == "parameterSetCopy") {
      //skip first attempt at this (new xdaq doing causing it at init?)
//      if (!configurationInit_) configurationInit_=true;
//      else initEDMConfiguration();
    }
  }
}

//______________________________________________________________________________
void Processor::reconfigureEP() {
  //reset cfg in case DQM FM already retrieved configuration at FM configure for it's own setup
  //configurationInitialized_=false;

  //(re)init semaphore
  try {
    if (semaphore_==0) {
      semaphore_ = (sem_t*)mmap(NULL, sizeof(sem_t),
			    PROT_READ | PROT_WRITE,
			    MAP_ANONYMOUS | MAP_SHARED, 0, 0);
      if (!semaphore_) {
        perror("out of memory\n");
      cout << "error with mmap\n";
      }

    } else {sem_destroy(semaphore_);}

    if (semaphore_) {
      sem_init(semaphore_,true,-1);
      int retsemval;
      //roll semapphore to 0 if not
      sem_getvalue(semaphore_, &retsemval);
      while (retsemval>0) {sem_wait(semaphore_);sem_getvalue(semaphore_, &retsemval);}
      while (retsemval<0) {sem_post(semaphore_);sem_getvalue(semaphore_, &retsemval);}
      //cout << "initialized sem :"<<retsemval<<endl;
    }
  }
  catch (...) {LOG4CPLUS_ERROR(getApplicationLogger(),"Exception: could not initialize semaphore");}

  masterStat_ = masterStatT(); 
  //setup shared parameters with the wrapper (static ones)
  wCfg_.ispace=applicationInfoSpace_;
  wCfg_.urn=getApplicationDescriptor()->getURN();
  wCfg_.anonymousPipe_[0]=anonymousPipe_[0];
  wCfg_.anonymousPipe_[1]=anonymousPipe_[1];
  wCfg_.timeoutOnStop=timeoutOnStop_.value_;
  wCfg_.monitorSleepSec=monSleepSec_.value_;
  wCfg_.lsTimeOut = lsTimeOut_.value_;
  wCfg_.idleRestartTime = (int)(idleRestartTime_.value_);
  wCfg_.instance = getApplicationDescriptor()->getInstance();
  wCfg_.hasSharedMemory = hasShMem_.value_;
  wCfg_.hasModuleWebRegistry = hasModuleWebRegistry_.value_;
  wCfg_.hasServiceWebRegistry = hasServiceWebRegistry_.value_;
  wCfg_.haveScalers = hasScalersService_.value_;
  wCfg_.nbSubProcesses = nbSubProcesses_.value_;
  wCfg_.detachTimeout = detachTimeout_.value_;
  wCfg_.logLevel = getApplicationLogger().getLogLevel();
  wCfg_.semaphore=semaphore_;
  //reset info from child processes..
  epModuleDescs_.reset();
  epModuleIndex_.reset();
  epStateIndex_.reset();
  
  //reset trigger report info
  lastReceivedTrLs_=0;
  int tls = 0;
  trhHistoryIndex_=0;
  if (trhSumAcc_) { delete trhSumAcc_;trhSumAcc_=0;}
  trhSumAcc_ = new TriggerReportHelpers();
  for (unsigned int i=0;i<trh_.size();i++) delete trh_[i];
  trh_.clear();
  for (unsigned int i=0;i<trhAcc_.size();i++) delete trhAcc_[i];
  trhAcc_.clear();
  for (unsigned int i=0;i<trhHistorySum_.size();i++) delete trhHistorySum_[i];
  trhHistorySum_.clear();
  lsUpdated_.clear();
  
  for (unsigned int i=0;i<keepTrhHistory_;i++) {
    trhHistorySum_.push_back(new TriggerReportHelpers());
  }

  for (unsigned int i=0;i<nbSubProcesses_.value_;i++) {
    trh_.push_back(new TriggerReportHelpers());
    trhAcc_.push_back(new TriggerReportHelpers());
    lsUpdated_.push_back(tls);
  }
  //will be initialized on first tr receive from slaves

  //makeStaticInfo();
  inForcedKillMask_=0;
}

//______________________________________________________________________________
bool Processor::configuring(toolbox::task::WorkLoop* wl)
{
  LOG4CPLUS_INFO(getApplicationLogger(),"Start configuring ...");
  pthread_mutex_lock(&start_lock_);
  pthread_mutex_lock(&start_lock2_);
  reconfigureEP();
  pthread_mutex_unlock(&start_lock2_);
  pthread_mutex_unlock(&start_lock_);
  fsm_.fireEvent("ConfigureDone",this);
  LOG4CPLUS_INFO(getApplicationLogger(),"Finished configuring!");
  localLog("-I- Configuration completed");

  return false;
}

//______________________________________________________________________________
bool Processor::enabling(toolbox::task::WorkLoop* wl)
{
  LOG4CPLUS_INFO(getApplicationLogger(), "Starting with Run Number "<< runNumber_.value_);
  cout << "Starting with Run Number " << runNumber_.value_ << endl;

  startedCounter_=0;
  //update configuration if FM did not invoke it

  /*bool ret = initEDMConfiguration();
  if (!ret) {
    //back to halted state if configuration problem
    localLog("-I- Start aborted: Configuration error. "+configString_.value_);
    LOG4CPLUS_ERROR(getApplicationLogger(), "Start aborted. Configuration error.");
    fsm_.fireEvent("EnableAbort",this);
    return false;
  }*/
  pthread_mutex_lock(&start_lock_);
  pthread_mutex_lock(&start_lock2_);

  //reset subprocess descriptors..
  subs_.clear();
  subs_.resize(nbSubProcesses_.value_);
  spMStates_.resize(nbSubProcesses_.value_);
  spmStates_.resize(nbSubProcesses_.value_);

  for(unsigned int i = 0; i < spMStates_.size(); i++)
  {
    spMStates_[i] = 0; 
    spmStates_[i] = 0; 
  }
  pthread_mutex_unlock(&start_lock2_);
  pthread_mutex_unlock(&start_lock_);


  pid_t retval = -1;
  for(unsigned int i=0; i<nbSubProcesses_.value_; i++)
  {
    pthread_mutex_lock(&start_lock_);
    pthread_mutex_lock(&start_lock2_);
    subs_[i]=SubProcess(i+msgQueueOffset_.value_*1000,retval); //create sub structure
    pthread_mutex_unlock(&start_lock_);
    pthread_mutex_unlock(&start_lock2_);
    
    retval = subs_[i].forkNew();
    if(retval==0)
    {
      //begin child process stuff
      spawnChild(i);
      return false;
      //end child process stuff
    }
    else {
      readEDMConfiguration();
    }
  }
  //start threads for child process IPC (will start much before EP in child)
  ::sleep(1);
  //wlSummarize_=0;
  startSupervisorWorkLoop();

  fsm_.fireEvent("EnableDone",this);
  startSummarizeWorkLoop();
  
  localLog("-I- Start completed");
  LOG4CPLUS_INFO(getApplicationLogger(),"Finished enabling!");
  return false;
}

//______________________________________________________________________________
bool Processor::halting(toolbox::task::WorkLoop* wl)
{
  //used only to stop master process
  LOG4CPLUS_INFO(getApplicationLogger(),"Start halting ...");

  stopSlavesAndAcknowledge();

  LOG4CPLUS_INFO(getApplicationLogger(),"Finished halting!");
  fsm_.fireEvent("HaltDone",this);

  localLog("-I- Halt completed");
  return false;
}

bool Processor::shuttingdown(toolbox::task::WorkLoop* wl) {
  while (detachedRunning_) {
    localLog("-I- Shutdown in progress: waiting for detached process to finish");
    sleep(1);
  }
  fsm_.fireEvent("ShutdownDone",this);
    localLog("-I- Shutdown done");
  return false;
}

//______________________________________________________________________________
void Processor::stopSlavesAndAcknowledge()
{
  MsgBuf msg(0,MSQM_MESSAGE_TYPE_STOP);
  MsgBuf msgRec(0,MSQS_MESSAGE_TYPE_STOP);

  pthread_mutex_lock(&stop_lock_);
  if (isStopping_) {//quit if already stopping(web callback vs "Stop")
    pthread_mutex_unlock(&stop_lock_);
    return;
  }
  isStopping_=true;
  pthread_mutex_unlock(&stop_lock_);

  unsigned int nAliveInitial=0, nGood=0, nBad=0, nDied=0;
  char * status = new char[subs_.size()];
  for (unsigned int i=0;i<subs_.size();i++) status[i]=0;

  for(unsigned int i = 0; i < subs_.size(); i++)
  {
    if(subs_[i].alive()>0){
      if (semaphore_!=0) {
        sem_post(semaphore_);
      }
      nAliveInitial++;
      status[i]=0;
    }
  }
  unsigned int msgTimeout=1000000*timeoutOnStop_.value_+100000;
  
  //todo:report if all successfully stopped in gui
  while (msgTimeout>0) {
    ::usleep(100000);
    msgTimeout-=100000;
    for (unsigned int i=0;i<subs_.size();i++) {
      if (!status[i]) {
	try {
	  unsigned int ret;
	  pthread_mutex_lock(&sup_queue_lock_);
	  ret = subs_[i].rcvNonBlockingSup(msgRec); 
	  pthread_mutex_unlock(&sup_queue_lock_);
	  if(ret==MSQS_MESSAGE_TYPE_STOP) {
	    nGood++;
	    status[i]=1;//done
	    LOG4CPLUS_INFO(getApplicationLogger(),"stop was good!");
	  }
	  else if (subs_[i].alive()<=0) {
	    nDied++;
	    status[i]=2;//died
	    LOG4CPLUS_ERROR(getApplicationLogger(),"Child process died before replying");
	  }
	}
	catch(dqmevf::Exception &e){
	  pthread_mutex_unlock(&sup_queue_lock_);
	  if (subs_[i].alive()<=0) {
	    nBad++;
	    status[i]=3;//dead+exception
	    LOG4CPLUS_ERROR(getApplicationLogger(),"Crashed subprocess");
	  }
	}
      }
    }
    if (nBad+nGood+nDied>=nAliveInitial || detachTimeout_.value_!=0) break;
  }
  if (detachTimeout_.value_==0) {
    if (nGood && msgTimeout>500000) usleep(500000); //wait for termination after reply
    //kill everyone
    for (unsigned int i=0;i<subs_.size();i++) {
      //make sure to terminate the child if alive
      int ret = kill(subs_[i].pid(),9);//kill -9 making sure child processes are dead
      if (!ret  || (ret==-1 && errno==ESRCH)) continue; //it's dead
      else cout << "Warning: child process not killed with signal 9! reason"<< strerror(errno) <<"\n";
    }
  }
  else {
    //put to detached list..
    for (unsigned int i=0;i<subs_.size();i++) {
      detached_.push_back(std::pair<int,int>(subs_[i].pid(),detachTimeout_.value_));
      subs_[i].setStatus(-1000);
      detachedRunning_=true;
    }
  }
  //wait shutdowns if stop is not forced

  if (nBad || nDied) {
    std::string reasonForFailedState_ = "STOPPING problem: some child Instances couldn't shutdown cleanly";
    //TODO: display status of each one, report returned messages
    LOG4CPLUS_WARN(getApplicationLogger(),reasonForFailedState_);
    //}
  }

  //disconnect from the queue
  //(handled by supervisor thread as well)
  for (unsigned int i=0;i<subs_.size();i++) subs_[i].disconnect();
  //configurationInitialized_=false;

  pthread_mutex_lock(&stop_lock_);
  isStopping_=false;
  pthread_mutex_unlock(&stop_lock_);
  delete status;

}



//______________________________________________________________________________
void Processor::defaultWebPage(xgi::Input  *in, xgi::Output *out)
  throw (xgi::exception::Exception)
{
webHelper_->importStaticHTML(out, 
		getApplicationDescriptor()->getContextDescriptor()->getURL() + "/"
	        + getApplicationDescriptor()->getURN() + "/"
	);
}

//______________________________________________________________________________
void Processor::moduleWeb(xgi::Input *in, xgi::Output *out)
{
  pthread_mutex_lock(&start_lock_); 
  Cgicc cgi(in);
  std::vector<FormEntry> el1;
  cgi.getElement("module",el1);

  ostringstream ost;
  //insert put our internal query (asking always a first subprocess)
  ost << "&process="<< subs_[0].pid() << "&method=moduleWeb";
  if (el1.size()!=0) ost << "&module=" << el1[0].getValue();
  //attach the rest
  ost <<"%;";
  ost << in->cin();

  xgi::Input newInput(ost.str().c_str(),ost.str().size());
  subWeb(&newInput,out);
  pthread_mutex_unlock(&start_lock_); 
}

//______________________________________________________________________________
void Processor::serviceWeb(xgi::Input *in, xgi::Output *out)
{
  pthread_mutex_lock(&start_lock_); 
  Cgicc cgi(in);
  std::vector<FormEntry> el1;
  cgi.getElement("service",el1);
  
  ostringstream ost;
  //insert put our internal query (asking always a first subprocess)
  ost << "&process="<< subs_[0].pid() << "&method=serviceWeb";
  if (el1.size()!=0) ost << "&service=" << el1[0].getValue();
  //attach the rest
  ost <<"%;";
  ost << in->cin();

  xgi::Input newInput(ost.str().c_str(),ost.str().size());
  subWeb(&newInput,out);
  pthread_mutex_unlock(&start_lock_); 
}

//______________________________________________________________________________
void Processor::spotlightWebPage(xgi::Input  *in, xgi::Output *out)
  throw (xgi::exception::Exception)
{
  pthread_mutex_lock(&start_lock_);
  std::string urn = getApplicationDescriptor()->getURN();

  *out << "<!-- base href=\"/" <<  urn
       << "\"> -->" << std::endl;
  *out << "<html>"                                                   << std::endl;
  *out << "<head>"                                                   << std::endl;
  *out << "<link type=\"text/css\" rel=\"stylesheet\"";
  *out << " href=\"/evf/html/styles.css\"/>"     	             << std::endl;
  *out << "<title>" << getApplicationDescriptor()->getClassName() 
       << getApplicationDescriptor()->getInstance() 
       << " MAIN</title>"     << std::endl;
  *out << "</head>"                                                  << std::endl;
  *out << "<body>"                                                   << std::endl;
  *out << "<table border=\"0\" width=\"100%\">"                      << std::endl;
  *out << "<tr>"                                                     << std::endl;
  *out << "  <td align=\"left\">"                                    << std::endl;
  *out << "    <img"                                                 << std::endl;
  *out << "     align=\"middle\""                                    << std::endl;
  *out << "     src=\"/evf/images/spoticon.jpg\""		     << std::endl;
  *out << "     alt=\"main\""                                        << std::endl;
  *out << "     width=\"64\""                                        << std::endl;
  *out << "     height=\"64\""                                       << std::endl;
  *out << "     border=\"\"/>"                                       << std::endl;
  *out << "    <b>"                                                  << std::endl;
  *out << getApplicationDescriptor()->getClassName() 
       << getApplicationDescriptor()->getInstance()                  << std::endl;
  *out << "      " << fsm_.stateName()->toString()                   << std::endl;
  *out << "    </b>"                                                 << std::endl;
  *out << "  </td>"                                                  << std::endl;
  *out << "  <td width=\"32\">"                                      << std::endl;
  *out << "    <a href=\"/urn:xdaq-application:lid=3\">"             << std::endl;
  *out << "      <img"                                               << std::endl;
  *out << "       align=\"middle\""                                  << std::endl;
  *out << "       src=\"/hyperdaq/images/HyperDAQ.jpg\""             << std::endl;
  *out << "       alt=\"HyperDAQ\""                                  << std::endl;
  *out << "       width=\"32\""                                      << std::endl;
  *out << "       height=\"32\""                                     << std::endl;
  *out << "       border=\"\"/>"                                     << std::endl;
  *out << "    </a>"                                                 << std::endl;
  *out << "  </td>"                                                  << std::endl;
  *out << "  <td width=\"32\">"                                      << std::endl;
  *out << "  </td>"                                                  << std::endl;
  *out << "  <td width=\"32\">"                                      << std::endl;
  *out << "    <a href=\"/" << urn << "/\">"                         << std::endl;
  *out << "      <img"                                               << std::endl;
  *out << "       align=\"middle\""                                  << std::endl;
  *out << "       src=\"/evf/images/metal029.jpg\""		     << std::endl;
  *out << "       alt=\"main\""                                      << std::endl;
  *out << "       width=\"32\""                                      << std::endl;
  *out << "       height=\"32\""                                     << std::endl;
  *out << "       border=\"\"/>"                                     << std::endl;
  *out << "    </a>"                                                 << std::endl;
  *out << "  </td>"                                                  << std::endl;
  *out << "</tr>"                                                    << std::endl;
  *out << "</table>"                                                 << std::endl;

  *out << "<hr/>"                                                    << std::endl;
  summaryWeb(in,out);
  *out << "</table>"                                                 << std::endl;
  
  *out << "<br><textarea rows=" << 10 << " cols=80 scroll=yes>"      << std::endl;

  *out << friendlyPythonCfg_                                         << std::endl;
  *out << "</textarea><P>"                                           << std::endl;
  
  *out << "</body>"                                                  << std::endl;
  *out << "</html>"                                                  << std::endl;
  pthread_mutex_unlock(&start_lock_);
}

//______________________________________________________________________________
void Processor::summaryWeb(xgi::Input *in, xgi::Output *out)
{
  TriggerReportStatic * trs =0;
  if (trhSumAcc_)
    trs = trhSumAcc_->getPackedTriggerReportAsStruct();

  *out << "<table>"                                        		<< std::endl;
  *out << "<tr valign=\"top\">"                          		<< std::endl;

  if (!trs || trs->trigPathsInMenu==0) {
    *out << "<td>HLT Report not yet received</td>" << std::endl;
  }
  else {
    // trigger summary table
    *out << "<td>" 							<< std::endl;
    *out << "<table border=1 bgcolor=\"#CFCFCF\">" 			<< std::endl;
    *out << "  <tr>"							<< std::endl;
    *out << "    <th colspan=7>"					<< std::endl;
    *out << "      Trigger Summary up to LS " << lastReceivedTrLs_      << std::endl;
    *out << "    </th>"							<< std::endl;
    *out << "  </tr>"							<< std::endl;
    *out << "  <tr >"							<< std::endl;
    *out << "    <th >Path</th>"					<< std::endl;
    *out << "    <th >Exec</th>"					<< std::endl;
    *out << "    <th >Pass</th>"					<< std::endl;
    *out << "    <th >Fail</th>"					<< std::endl;
    *out << "    <th >Except</th>"					<< std::endl;
    *out << "  </tr>"							<< std::endl;
    for(int i=0; i<trs->trigPathsInMenu; i++) {
      PathSummaryStatic * pss = &(trs->trigPathSummaries[i]);
      *out << "  <tr>" 							<< std::endl;
      *out << "    <td>"<< std::string(pss->name) << "</td>"	<< std::endl;
      *out << "    <td>" <<pss->timesRun << "</td>"			<< std::endl;
      *out << "    <td>" << pss->timesPassed << "</td>"			<< std::endl;
      *out << "    <td >" << pss->timesFailed << "</td>"		<< std::endl;
      *out << "    <td ";
      if(pss->timesExcept !=0)
	*out << "bgcolor=\"red\""		      			<< std::endl;
      *out << ">" << pss->timesExcept << "</td>"			<< std::endl;
      *out << "  </tr >"						<< std::endl;
    }
    *out << "</table>" << std::endl;
    *out << "</td>" 							<< std::endl;
  }

  *out << "<td>" << std::endl;
  //Module details printout from subprocess
  if (subs_.size())
    subWebRetriever(out,std::string("epModuleDescs"),subs_[0].pid(),-1,0);
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;
  *out << "</table>" << std::endl;
}

//______________________________________________________________________________
void Processor::microState(xgi::Input *in,xgi::Output *out)
{
  std::string urn = getApplicationDescriptor()->getURN();
  pthread_mutex_lock(&start_lock_);
  try{
    //master process status
    *out << "<tr>\n";
    *out << "<td>" << fsm_.stateName()->toString() << "</td>\n"
	 << "<td>M</td>\n" 
         << "<td><a href=\"/" << urn << "/procStat\">" << getpid() <<"</a></td>\n"
	 << "<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>\n"
	 << "<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>\n"
	 << "<td >"<< masterStat_.nbAccepted<<"/"<< masterStat_.nbProcessed <<"/"<<masterStat_.nbDQMProcessed<<"</td>\n"
	 << "<td>"<< lastReceivedTrLs_ <<"</td>\n";
    if (!inForcedKillMask_) {
      if (fsm_.stateName()->toString()=="Halted")
	    *out << "<td><button type=\"button\" onclick=\"callManual(0)\">Configure</button></td>\n";
      if (fsm_.stateName()->toString()=="Ready")
	    *out << "<td><button type=\"button\" onclick=\"callManual(1)\">Enable</button></td>\n";
      if (isEnabled())
	    *out << "<td><button type=\"button\" onclick=\"callManual(2)\">Halt</button></td>\n";
    }

    //*out << "<td>" /*<< fwepWrapper_.getScalersUpdates()*/ << "</td>";
    *out << "</tr>\n";
    if(1) 
    {

      for(unsigned int i = 0; i < subs_.size(); i++)
      {
	try{
	  if(subs_[i].alive()>0)
	  {
	    pthread_mutex_lock(&pickup_lock_);
	    pid_t mypid = subs_[i].pid();
	    int myMs = subs_[i].params().Ms;
	    int myms = subs_[i].params().ms;
	    
	    *out << "<tr>";
	    *out   << "<td  bgcolor=\"#00FF00\" id=\"a" << i << "\">Alive</td>\n";
	    *out   <<  "<td>S</td>\n";
	    //pid+link
	    *out   << "<td>"
	           << "<a id=\"p"<< i << "\" href=\"SubWeb?process=" << subs_[i].pid() << "&method=procStat\">" 
	           << subs_[i].pid() <<"</a>" << "</td>\n"
	           << "<td>"; 
	    pthread_mutex_unlock(&pickup_lock_);
	    //unlocked to allow supervisor thread to cleanup if dead
	    subWebRetriever(out, std::string("epStateIndex"), mypid, myMs, 0);
	    *out << "</td>\n"
	         << "<td>\n";
	    subWebRetriever(out, std::string("epModuleIndex"), mypid, myms, 0);
	    *out << "</td>\n";
	    pthread_mutex_lock(&pickup_lock_);
	    *out <<"<td>"
	         << subs_[i].params().nba << "/" << subs_[i].params().nbp << "/" << subs_[i].params().dqm; 
	    if (subs_[i].params().nbp>1) {
	      *out << " (" << int(float(subs_[i].params().nba)/float(subs_[i].params().nbp)*100.) <<"%)</td>\n" ;
	    }
	    else {
	      *out << "</td>\n" ;
	    }
	    *out << "<td>" << subs_[i].params().ls <<"</td>";

            if (isEnabled()) {
	      if (!(inForcedKillMask_&(1<<i)))
	        *out << "<td><button id=\"ResetButton" << subs_[i].pid() << "\" type=\"button\" onclick=\"callReset("
                     << subs_[i].pid() << ")\">Reset</button></td>";
	      else
	        *out << "<td><button id=\"ResetButton" << subs_[i].pid() << "\" type=\"button\" bgcolor=\"#bbaabb\">"
                     << "Resetting</button></td>";
            }

	    *out << "\n</tr>";
	    pthread_mutex_unlock(&pickup_lock_);
	  }
	  else 
	  {
	    pthread_mutex_lock(&pickup_lock_);
	    *out << "<tr><td id=\"a"<< i << "\" ";
	    if(subs_[i].alive()==-1000)
	      *out << " bgcolor=\"#bbaabb\">NotInitialized";
	    else
	      *out << (subs_[i].alive()==0 ? " bgcolor=\"#0033cc\">Done" : " bgcolor=\"#FF0000\">Dead");
	    *out <<"</td>";
	    *out << "<td>S</td>";
	    *out <<"<td id=\"p"<< i << "\">" << subs_[i].pid() <<"</td>";
	    *out <<"<td >" << subs_[i].reasonForFailed();
	    if(subs_[i].alive()<=0 && subs_[i].alive()!=-1000) 
	    {
	      if(autoRestartSlaves_ && isEnabled()) *out << " R:" << subs_[i].countdown() << " s";
	      else *out << " ";
	    }
	    *out << "</td>";

            *out <<"<td/>";

   	    *out <<"<td>" << subs_[i].params().nba << "/" << subs_[i].params().nbp << "/" << subs_[i].params().dqm;
	    if (subs_[i].params().nbp>1) {
	     *out << " (" << int(float(subs_[i].params().nba)/float(subs_[i].params().nbp)*100.) <<"%)</td>\n" ;
	    }
	    else *out << " (" << "-" <<"%)</td>\n" ;

	    *out << "<td>" << subs_[i].params().ls  << "</td>";
	    if (isEnabled()) {

	      if (!(inForcedKillMask_&(1<<i)))
	        *out << "<td><button id=\"ResetButton" << subs_[i].pid() << "\" type=\"button\" onclick=\"callReset("
                     << subs_[i].pid() << ")\">Reset</button></td>";
	      else
	        *out << "<td><button id=\"ResetButton" << subs_[i].pid() << "\" type=\"button\" bgcolor=\"#bbaabb\">"
                     << "Resetting</button></td>";
	    }
	    *out << "</tr>";
	    pthread_mutex_unlock(&pickup_lock_);
	  }
	}
	catch(dqmevf::Exception &e){
	  pthread_mutex_unlock(&pickup_lock_);
	  pthread_mutex_lock(&pickup_lock_);
	  *out << "<tr>";
	  *out << "<td id=\"a"<< i << "\" " 
	       <<"bgcolor=\"#FFFF00\">NotResp</td><td>S</td>"
	       << "<td id=\"p"<< i << "\">"
	       << subs_[i].pid()<<"</td>";
	  *out <<"<td>"<< e.what() << "</td>";

	  *out << "</tr>";
	  pthread_mutex_unlock(&pickup_lock_);
	}
      }
    }
  }
  catch(dqmevf::Exception &e)
  {
    LOG4CPLUS_INFO(getApplicationLogger(),"dqmevf::Exception caught in microstate - " << e.what());
  }
  catch(cms::Exception &e)
  {
    LOG4CPLUS_INFO(getApplicationLogger(),"cms::Exception caught in microstate - " << e.what()); 
  }
  catch(std::exception &e)
  {
    LOG4CPLUS_INFO(getApplicationLogger(),"std::Exception caught in microstate - " << e.what());    
  }
  catch(...)
  {
    LOG4CPLUS_INFO(getApplicationLogger(),"unknown exception caught in microstate - ");
  }
  pthread_mutex_unlock(&pickup_lock_);
  pthread_mutex_unlock(&start_lock_);

}

//main routine to retrieve status information from sub process (for web UI js)
//______________________________________________________________________________
void Processor::subWeb(xgi::Input  *in, xgi::Output *out)
{
  using namespace cgicc;
  pid_t pid = 0;
  std::ostringstream ost;
  ost << "&";

  //parse the cgicc input for our parameters and prepare for cgi forwarding
  Cgicc cgi(in);
  internal::MyCgi *mycgi = (internal::MyCgi*)in;
  for(std::map<std::string, std::string, std::less<std::string> >::iterator mit = 
	mycgi->getEnvironment().begin();
      mit != mycgi->getEnvironment().end(); mit++)
    ost << mit->first << "%" << mit->second << ";";

  std::vector<FormEntry> els = cgi.getElements() ;
  
  //find method
  std::vector<FormEntry> method_el;
  cgi.getElement("method",method_el);
  if(method_el.size()==0) return;//abort if no method given
  std::string meth = method_el[0].getValue();
  
  //find process
  std::vector<FormEntry> proc_el;
  cgi.getElement("process",proc_el);
  if(proc_el.size()==0) return;//abort if no subprocess pid given
  else {
    std::string pmod = proc_el[0].getValue();
    pid = atoi(pmod.c_str()); // get the process id to be polled
  }
  
  //find (optional) parameter 1 (integer)
  int par1=-1;
  std::vector<FormEntry> par1_el;
  cgi.getElement("parameter",par1_el);
  if(par1_el.size()!=0)
    par1=atoi((par1_el[0].getValue()).c_str());

  pthread_mutex_lock(&start_lock_);
  subWebRetriever(out,meth,pid,par1, &ost);
  pthread_mutex_unlock(&start_lock_);
}

//retrieve data from subprocess directly from Processor
void Processor::subWebRetriever(xgi::Output *out, std::string method, pid_t childPid, int par1, std::ostringstream * cgiPassThrough = 0 ) {
   bool tryToCache=false;
   //check cacheable methods (static data)
   if (method=="epModuleDescs") {
     if (epModuleDescs_.cached) {
       epModuleDescs_.fill(out);
       return;
     }
     else tryToCache=true;
   }
   else if (method=="epStateIndex") {
     if (epStateIndex_.cached) {
       epStateIndex_.fill(out,par1);
       return;
     }
     else tryToCache=true;
   }
   else if (method=="epModuleIndex") {
     if (epModuleIndex_.cached) {
       epModuleIndex_.fill(out,par1);
       return;
     }
     else tryToCache=true;
   }
  //check process state/pid match
  unsigned int i = 0;
  for(; i < subs_.size(); i++)
    if(subs_[i].pid()==childPid) break;

  if(i>=subs_.size()){ //process was not found, let the browser know //todo:sanitycheck
    if (method!="epStateIndex" && method!="epModuleIndex")
      *out << "ERROR 404 : Process " << childPid << " Not Found !" << std::endl;
     else *out << " -NI- " << std::endl;//shorter version(no cache)
     return;
   }
   if(subs_[i].alive() != 1){
     if (method!="epStateIndex" && method!="epModuleIndex")
       *out << "ERROR 405 : Process " << childPid << " Not Alive !" << std::endl;
     else *out << " -NA- " << std::endl;//shorter version(no cache)
     return;
   }

   std::ostringstream * ost;
   if (cgiPassThrough) {
	   //use string provided by caller
	   ost = cgiPassThrough;
   }
   else {
	//put directly invoked method
	ost = new std::ostringstream;
	*ost << "&" << method;
   }

   //request from subprocess
   MsgBuf msg1(method.length()+ost->str().length(),MSQM_MESSAGE_TYPE_WEB);
   strcpy(msg1->mtext,method.c_str());
   strcpy(msg1->mtext+method.length(),ost->str().c_str());
   
   subs_[i].postMon(msg1);
   MsgBuf msg(MAX_MSG_SIZE,MSQS_MESSAGE_TYPE_WEB);
   bool done = false;
   vector<char *>pieces;
   vector<unsigned> pieceSizes;

   int sizecount=0;
   //read back
   usleep(5000);
   while(!done){
     try {
       int tout=10;
       do {
	 if (!tout) throw dqmevf::Exception("","","",0,"");
         //subs_[i].rcvMon(msg);
         int monret = subs_[i].rcvNonBlockingMon(msg);
         if (monret == (unsigned long)MSQS_MESSAGE_TYPE_WEB) break;
         usleep(10000);
       } while (tout--);
     } catch (...) {sizecount=0;break;}
     unsigned int nbytes = atoi(msg->mtext);
     if(nbytes < MAX_PIPE_BUFFER_SIZE) done = true;
     char *buf= new char[nbytes];
     read(anonymousPipe_[PIPE_READ],buf,nbytes);
     pieces.push_back(buf);
     pieceSizes.push_back(nbytes);
     sizecount+=nbytes;
   }

   //try to cache ("passthrough")
   if (tryToCache) {
     if (method=="epModuleDescs") {
       bool success = epModuleDescs_.cacheNew(pieces,pieceSizes,sizecount);
       if (success) epModuleDescs_.fill(out);
     }
     else if (method=="epModuleIndex") {
       /*bool success =*/ epModuleIndex_.cacheNew(pieces,pieceSizes,sizecount);
       /*if (success)*/ epModuleIndex_.fill(out,par1);
     }
     else if (method=="epStateIndex") {
       /*bool success =*/ epStateIndex_.cacheNew(pieces,pieceSizes,sizecount);
       /*if (success)*/ epStateIndex_.fill(out,par1);
     }
   }
   else {//just write back
     for(unsigned int j = 0; j < pieces.size(); j++){
       *out<<pieces[j];    // chain the buffers into the output strstream
     }
   }
   //cleanup
   for(unsigned int j = 0; j < pieces.size(); j++){
     delete[] pieces[j]; //make sure to release all buffers used for reading the pipe
   }
   delete ost;
}

//______________________________________________________________________________
void Processor::initiateReset(xgi::Input  *in, xgi::Output *out) {
  //kills a child processes

  int killpid=0;
  //read pid form element from request
  Cgicc cgi(in);
  for(const_form_iterator el =
      cgi.getElements().begin();
      el != cgi.getElements().end(); el++)
    if (std::string("pid")==el->getName()) killpid= atoi(el->getValue().c_str());

  if (killpid<=0) return;
  pthread_mutex_lock(&start_lock_);

  if (isEnabled()) {
    localLog("-I- Manual Reset initiated");
    for (unsigned int i=0;i<subs_.size();i++) {
      if (inForcedKillMask_&(1<<i)) continue;
      if (killpid!=subs_[i].pid()) continue;
      inForcedKillMask_ |= 1<<i;
      if (subs_[i].alive()) {
        int ret = kill(subs_[i].pid(),9);//kill -9
        if (!ret  || (ret==-1 && errno==ESRCH)) continue;
        else LOG4CPLUS_ERROR( getApplicationLogger(),"Warning: child process not killed. Error:"<< strerror(errno));
      }
    }
  }
  pthread_mutex_unlock(&start_lock_);
}
//______________________________________________________________________________
//manual state change callbacks..
void Processor::manualConfigure(xgi::Input  *in, xgi::Output *out) {
 fsm_.fireEvent("Configure",this); 
}
void Processor::manualEnable(xgi::Input  *in, xgi::Output *out) {
 fsm_.fireEvent("Enable",this);
}
void Processor::manualHalt(xgi::Input  *in, xgi::Output *out){
 fsm_.fireEvent("Halt",this);
}
//TODO:
void Processor::manualRecover(xgi::Input  *in, xgi::Output *out) {
 fsm_.fireEvent("Recover",this);
}
//______________________________________________________________________________
void Processor::startSummarizeWorkLoop() throw (dqmevf::Exception)
{
  try {
    if (!wlSummarize_)

      wlSummarize_=
	toolbox::task::getWorkLoopFactory()->getWorkLoop("Summary", "waiting");

    if (!wlSummarize_->isActive()) wlSummarize_->activate();
    //else return;

    asSummarize_ = toolbox::task::bind(this,&Processor::summarize, "Summary");

    wlSummarize_->submit(asSummarize_);
    wlSummarizeActive_ = true;
  }
  catch (xcept::Exception& e) {
    std::string msg = "Failed to start workloop 'Summarize'.";
    XCEPT_RETHROW(dqmevf::Exception,msg,e);
  }
}

//______________________________________________________________________________
bool Processor::summarize(toolbox::task::WorkLoop* wl)
{
  MsgBuf msg(sizeof(TriggerReportStatic),MSQS_MESSAGE_TYPE_TRR);

  //throttle
  sleep(1);
  int lockret = pthread_mutex_trylock(&summary_queue_lock_);
  if (lockret!=0) return true;

  //shutdown if end of run
  if(!isEnabled() || !wlSummarizeActive_){
    wlSummarizeActive_ = false;
    pthread_mutex_unlock(&summary_queue_lock_);
    return false;
  }

  //counters
  bool atLeastOneProcessUpdated = false;
  //int msgCount = 0;

  for (unsigned int i = 0; i < subs_.size(); i++) {

    if(subs_[i].alive()>0)
    {
      unsigned int ret;
      try {
	pthread_mutex_lock(&sup_queue_lock_);
	ret = subs_[i].rcvNonBlockingSup(msg);//receive from scalers queue
	pthread_mutex_unlock(&sup_queue_lock_);
      }
      catch(dqmevf::Exception &e)
      {
	pthread_mutex_unlock(&sup_queue_lock_);
	std::cout << "exception in msgrcv on " << i
	  << " " << subs_[i].alive() << " " << strerror(errno) << std::endl;
	continue;
      }
      if (ret!=MSQS_MESSAGE_TYPE_TRR) continue;
      //we got trigger report!
      TriggerReportStatic *trp = (TriggerReportStatic *)msg->mtext;
      unsigned int gotForLumi = trp->lumiSection;
      //std::cout << "getting trp " << i << " myL:"<<lsUpdated_[i] <<"slave:"<< gotForLumi<<std::endl;//debug
      if (gotForLumi==0) {
	//use to initialize names
	if (!lastReceivedTrLs_)
	  trhSumAcc_->replace(trp);
	trhAcc_[i]->replace(trp);
	trh_[i]->replace(trp);
	trhHistorySum_[0]->replace(trp);
      }
      if (lsUpdated_[i]>=gotForLumi || gotForLumi==0) {
	//do not expect decreased lumi updates (or mid-update), so do nothing
      } else {
	if (lsUpdated_[i]+1 != gotForLumi) {
	  //std::cout << "INFO:child process source has skipped lumisection!\n";
	}
	atLeastOneProcessUpdated=true;

	//add to cumulative block
	try {
	  trhSumAcc_->add(trp);//add even if late
	  trhAcc_[i]->add(trp);//add even if late
	}
	catch (...) {}
	  
	try {
	  trh_[i]->replace(trp);//put the last received one
	}
	catch (...) {}
	if (gotForLumi>lastReceivedTrLs_) {//new ls received 
	  unsigned int ldiff = gotForLumi - lastReceivedTrLs_;
	  lastReceivedTrLs_=gotForLumi;//update
	  if (ldiff>=trhHistorySum_.size()) {//reset history
	    for (unsigned int j=0;j<trhHistorySum_.size();j++) {
	      delete trhHistorySum_[j]; 
	      trhHistorySum_[j] = new TriggerReportHelpers();
	    }
	    //put new in sum trh
	    try {
	      trhHistorySum_[trhHistorySum_.size()-1]->replace(trp);
	    }
	    catch (...) {cout << "trigger inconsistency\n";}
	    trhHistoryIndex_=trhHistorySum_.size()-1;

	  }
	  else { //put in new place in history list
	    if (trhHistoryIndex_==trhHistorySum_.size()-1) trhHistoryIndex_=0;
	    else trhHistoryIndex_++;
	    try {
	      trhHistorySum_[trhHistoryIndex_]->replace(trp);
	    }
	    catch (...) {cout << "trigger inconsistency\n";}
	  }
	  //update to new lumis
	  lastReceivedTrLs_=gotForLumi;
	}
	else if (gotForLumi==lastReceivedTrLs_) { //received current LS
	  try {
	    trhHistorySum_[trhHistoryIndex_]->add(trp);
	  }
	  catch (...) {cout << "trigger inconsistency\n";}
	    }
	else if (gotForLumi<lastReceivedTrLs_) { //received last LS
	  unsigned int ldiff =  lastReceivedTrLs_ - gotForLumi;
	  if (ldiff<keepTrhHistory_) {
	    unsigned int newIndex = trhHistoryIndex_;
	    while (ldiff>trhHistoryIndex_) newIndex+=trhHistoryIndex_;
	    newIndex-=ldiff;
	    try {
	      trhHistorySum_[newIndex]->add(trp);
	    }
	    catch (...) {cout << "trigger inconsistency\n";}
	  }
	}
	//update child index and continue
	if (lsUpdated_[i]<gotForLumi) lsUpdated_[i]=gotForLumi;
      }
    }
  }
  if(atLeastOneProcessUpdated){
    //nbSubProcessesReporting_.value_ = msgCount;
    if (lastKnownLS_<lastReceivedTrLs_)
      lastKnownLS_=lastReceivedTrLs_;
  }
  pthread_mutex_unlock(&summary_queue_lock_);
  return true;
}

//______________________________________________________________________________
void Processor::startSupervisorWorkLoop()
{
  try {
    if (!wlSupervising_)
      wlSupervising_= toolbox::task::getWorkLoopFactory()->getWorkLoop("Supervisor", "waiting");

    if (!wlSupervising_->isActive()) wlSupervising_->activate();
    //else return;

    asSupervisor_ = toolbox::task::bind(this,&Processor::supervisor, "Supervisor");
    wlSupervising_->submit(asSupervisor_);
    supervising_ = true;
  }
  catch (xcept::Exception& e) {
    std::string msg = "Failed to start workloop 'Supervisor'.";
    XCEPT_RETHROW(dqmevf::Exception,msg,e);
  }
}

//______________________________________________________________________________
bool Processor::supervisor(toolbox::task::WorkLoop *)
{

  int ret1 = pthread_mutex_trylock(&stop_lock_);
  if (ret1) {
    ::sleep(superSleepSec_.value_);
    return true;
  }
  ret1 = pthread_mutex_trylock(&start_lock2_);
  if (ret1) {
    pthread_mutex_unlock(&stop_lock_);
    ::sleep(superSleepSec_.value_);
    return true;
  }
  
  bool running = isEnabled();

  //update degraded mode status
  if (isDegraded() && degradedCounter_<=0) fsm_.fireEvent("EnableDone",this);

  //monitor detached process
  if (!isStopping_)
  for(int it = 0; it < int(detached_.size()); it++) {
    unsigned int i = it;
    detached_[i].second--;
    std::pair<int,int> myProc = detached_[i];
    if (myProc.second==-1) kill(myProc.first,9);
    else {
       if (myProc.second>=0)
        if (!(myProc.second%5) || myProc.second+1==int(detachTimeout_.value_)) {
          ostringstream lost;
          lost << "-I- detached process "<< myProc.first <<" alive..(" << myProc.second << ")";
          localLog(lost.str().c_str());
        }
    }
    int sl;
    pid_t killedOrNot = waitpid(myProc.first,&sl,WNOHANG);
    if(killedOrNot==-1 || killedOrNot==myProc.first || myProc.second<=-3) {
      ostringstream lost;
      if (killedOrNot>0) {
        if (WIFEXITED(sl)) lost << "-I- detached process "<< myProc.first <<" finished";
        else lost << "-I- detached process "<< myProc.first <<" dead";
      }
      else if (killedOrNot==-1) lost << "-I- detached process "<< myProc.first << " was terminated previously";
      else lost << "-I- detached process "<< myProc.first <<": unknown post-mortem status";
      localLog(lost.str());
      detached_.erase(detached_.begin()+i);
      it--;//element was erased

    }
  
  }
  if (detached_.size()==0) detachedRunning_=false;
  
  //loop that checks if everyone is alive
  for(unsigned int i = 0; i < subs_.size(); i++)
  {
    if(subs_[i].alive()==-1000) continue;
    int sl;
    pid_t killedOrNot = waitpid(subs_[i].pid(),&sl,WNOHANG);
    //TODO:report to function manager if unexpected termination
    if(killedOrNot==subs_[i].pid()) subs_[i].setStatus((WIFEXITED(sl) != 0 ? 0 : -1));
    else {
      if (killedOrNot==-1) { 
	//if (sl!=9) //kill -9 warning ignored
	//  std::cout << "waitpid error:"<<sl<<"\n";
      }
      else
	subs_[i].setStatus(1);//alive
      continue;
    }
    try {
      subs_[i].disconnect();
    } catch (...) {}

    subs_[i].countdown()=slaveRestartDelaySecs_.value_;
    spMStates_[i] = 0;
    spmStates_[i] = 0;

    usleep(10000);
    int ret = pthread_mutex_trylock(&pickup_lock_);
    if (ret) {
      usleep(100000);
      ret = pthread_mutex_trylock(&pickup_lock_);
    }
    if (!ret) {
      std::ostringstream ost;
      if(subs_[i].alive()==0) ost << " exited(" << WEXITSTATUS(sl)<<")";
      else if(WIFSIGNALED(sl)!=0) ost << " teminated("<< WTERMSIG(sl)<<")";
      else ost << " stopped (other reason)";
      if (subs_[i].alive()==0)
	LOG4CPLUS_INFO(getApplicationLogger(),"Child " << ost.str());
      else 
	LOG4CPLUS_ERROR(getApplicationLogger(),"Child " << ost.str());

      subs_[i].setReasonForFailed(ost.str());

      std::ostringstream ost1;
      ost1 << "-E- Slave " << subs_[i].pid() << ost.str();
      localLog(ost1.str());
    }
    pthread_mutex_unlock(&pickup_lock_);
  }
  pthread_mutex_unlock(&stop_lock_);

  //restart child process sequence
  if(running) {
    //periodically check if dead slaves countdown has expired and restart them
    if(autoRestartSlaves_.value_ || inForcedKillMask_){
      int stopret = pthread_mutex_trylock(&stop_lock_); //lockout slave killing at stop while checking for restarts
      if (stopret) {
	pthread_mutex_unlock(&start_lock2_);
	return true;
      }
      if (!isStopping_ && isEnabled())
	for(unsigned int i = 0; i < subs_.size(); i++)
	{
	  bool fkill = ((1<<i)&inForcedKillMask_)!=0;

	  if (!fkill && !(autoRestartSlaves_.value_)) continue;

	  int ret1 = pthread_mutex_trylock(&start_lock_);
	  int ret2 = pthread_mutex_trylock(&summary_queue_lock_);

	  if (!ret1 && !ret2) { //lock acquired

	    if (subs_[i].alive() != 1){//postpone until waitpid sets new status
	      inForcedKillMask_&=~(1<<i);
	      if(subs_[i].countdown()-- == 0 || fkill)
	      {
		if (!fkill) masterStat_.storeToPrev();
		try {
		  reconfigureEP();
		} catch (...) {}
		/*if (!configurationInitialized_) {
		  localLog( "-E- Reset failed: configuration error ");
                  subs_[i].countdown()=slaveRestartDelaySecs_.value_;
		}*/
		{ //create new child process
		  pid_t retval = -1;
		  subs_[i]=SubProcess(i+msgQueueOffset_.value_*1000,retval); //create sub structure
		  bool oldSummarize = wlSummarizeActive_;//store for master process
		  wlSummarizeActive_=false;//setup before fork for child process
		  pid_t ret = subs_[i].forkNew();
		  if(ret==0)
		  {
		    spawnChild(i);
		    return false; // exit the supervisor loop (child process)
		  }
		  else //still in master
		  {
		    readEDMConfiguration();
		    //go to degraded mode if restarted mid-run (even if manually)
                    degradedCounter_=120;
		    if (startedCounter_>240 && isEnabledFully()) fsm_.fireEvent("Degrade",this);
                    cout << "tried degrading " << startedCounter_ << " " << isEnabledFully() << endl;

		    wlSummarizeActive_=oldSummarize;
		    std::ostringstream ost1;
		    ost1 << "-I- New Process " << ret << " forked for slot " << i;
		    localLog(ost1.str());
		  }
		}
	      }
	    }
	  }
	  pthread_mutex_unlock(&summary_queue_lock_);
	  pthread_mutex_unlock(&start_lock_);
	}
      pthread_mutex_unlock(&stop_lock_);
    }
  } //running

  
  MsgBuf msg1(0,MSQM_MESSAGE_TYPE_PRG);
  MsgBuf msg2(MAX_MSG_SIZE,MSQS_MESSAGE_TYPE_PRR);
  MsgBuf msg3(MAX_MSG_SIZE,MSQS_MESSAGE_TYPE_PRR);
  //statistics
  int nbdqm = masterStat_.nbDQMProcessedPrev;
  int nbp = masterStat_.nbProcessedPrev;
  int nba = masterStat_.nbAcceptedPrev;

  for(unsigned int i = 0; i < subs_.size(); i++)
  {
    if(subs_[i].alive()>0)
    {

      try{ //retrieve microstate
	int ret = pthread_mutex_trylock(&sup_queue_lock_);
	if (ret) continue;
	subs_[i].postSup(msg1);
	pthread_mutex_unlock(&sup_queue_lock_);
	::usleep(1000);//1 msec delay
	MsgBuf * newMsg = 0;

	//try two reads per cycle
	ret = pthread_mutex_trylock(&sup_queue_lock_);
	if (ret) continue;
	unsigned long retval2 = subs_[i].rcvNonBlockingSup(msg2);
	unsigned long retval3 = subs_[i].rcvNonBlockingSup(msg3);
	pthread_mutex_unlock(&sup_queue_lock_);

	//see what we received
	bool received_status = false;

	if (retval2 == (unsigned long)MSQS_MESSAGE_TYPE_PRR) {newMsg=&msg2;received_status=true;}
	if (retval3 == (unsigned long)MSQS_MESSAGE_TYPE_PRR) {newMsg=&msg3;received_status=true;}

	if (received_status) { //received child process status
	  prg* p = (struct prg*)((*newMsg)->mtext);

	  ret = pthread_mutex_trylock(&pickup_lock_);
	  if (ret) continue;
	  subs_[i].setParams(p);

	  spMStates_[i] = p->Ms;
	  spmStates_[i] = p->ms;

	  if(!subs_[i].inInconsistentState() &&
	      (p->Ms == edm::event_processor::sError
	       || p->Ms == edm::event_processor::sInvalid
	       || p->Ms == edm::event_processor::sStopping))
	  {
	    std::ostringstream ost;
	    ost << "edm::eventprocessor slot " << i << " process id "
	      << subs_[i].pid() << " not in Running state : Mstate="
	      <<  epStateIndex_.get(p->Ms) << " mstate="
	      << epModuleIndex_.get(p->ms)
	      << " - Look into possible error messages from HLT process";

	      subs_[i].setReportedInconsistent();
	  }
	  masterStat_.nbProcessed+=p->nbp;
	  masterStat_.nbAccepted+=p->nba;
	  masterStat_.nbDQMProcessed +=  p->dqm;
	  if(p->ls > lastKnownLS_) lastKnownLS_ = p->ls;
	  pthread_mutex_unlock(&pickup_lock_);
	}
      }
      catch(dqmevf::Exception &e){
	pthread_mutex_unlock(&sup_queue_lock_);
	pthread_mutex_unlock(&pickup_lock_);
	LOG4CPLUS_INFO(getApplicationLogger(),
	    "could not send/receive msg on slot "
	    << i << " - " << e.what());
      }

    }
    else {
      spMStates_[i]=0;
    }
  }
  for(unsigned int i = 0; i < subs_.size(); i++) {
    nba+=subs_[i].params().nba;
    nbp+=subs_[i].params().nbp;
    nbdqm+=subs_[i].params().dqm;
  }
  masterStat_.nbProcessed=nbp;
  masterStat_.nbAccepted=nba;
  masterStat_.nbDQMProcessed =  nbdqm;

  //end
  pthread_mutex_unlock(&start_lock2_);
  ::sleep(superSleepSec_.value_);
  if (degradedCounter_>0) degradedCounter_--;
  startedCounter_++;
  return true;
}

//______________________________________________________________________________
void Processor::updater(xgi::Input *in,xgi::Output *out)
{
  using namespace utils;

  *out << updaterStatic_;
  mDiv(out,"loads");
  uptime(out);
  cDiv(out);

  mDiv(out,"sh",hasShMem_.toString());
  mDiv(out,"mw",hasModuleWebRegistry_.toString());
  mDiv(out,"sw",hasServiceWebRegistry_.toString());
  mDiv(out,"ms",monSleepSec_.toString());
  mDiv(out,"lst",lsTimeOut_.toString());

  mDiv(out,"st",fsm_.stateName()->toString());
  mDiv(out,"ru",runNumber_.toString());
  mDiv(out,"nsl",nbSubProcesses_.value_);
  if(fsm_.stateName()->toString() != "Halted" && fsm_.stateName()->toString() != "halting"){
    mDiv(out,"hlt");
    *out << "<a href=\"" << configString_.toString() << "\">CMSSW Configuration</a>";//todo:get from slave
    cDiv(out);
    *out << std::endl;
  }
  else
    mDiv(out,"hlt","Not configured...");

  mDiv(out,"supwl",(supervising_ ? "Active" : "Not Initialized"));
  mDiv(out,"sumwl",(wlSummarizeActive_ ? "Active" : "Inactive"));

  //todo:improve on this..
  mDiv(out,"ll");
  *out << lastKnownLS_;// << "," <<trhSumAcc_. << ","
	 //todo//<< "," << fwepWrapper_.lastLumi().proc << "," << fwepWrapper_.lastLumi().acc;
  cDiv(out);

  mDiv(out,"lg");
  for(unsigned int i = logRingIndex_; i<logRingSize_; i++)
    *out << logRing_[i] << std::endl;
  if(logWrap_)
    for(unsigned int i = 0; i<logRingIndex_; i++)
      *out << logRing_[i] << std::endl;
  cDiv(out);
}

//______________________________________________________________________________
void Processor::procStat(xgi::Input *in, xgi::Output *out)
{
  dqmevf::utils::procStat(out);
}


//______________________________________________________________________________
std::string Processor::logsAsString()
{
  std::ostringstream oss;
  if(logWrap_)
    {
      for(unsigned int i = logRingIndex_; i < logRing_.size(); i++)
	oss << logRing_[i] << std::endl;
      for(unsigned int i = 0; i <  logRingIndex_; i++)
	oss << logRing_[i] << std::endl;
    }
  else
      for(unsigned int i = logRingIndex_; i < logRing_.size(); i++)
	oss << logRing_[i] << std::endl;
    
  return oss.str();
}

//______________________________________________________________________________
void Processor::localLog(std::string m)
{
  timeval tv;

  gettimeofday(&tv,0);
  tm *uptm = localtime(&tv.tv_sec);
  char datestring[256];
  strftime(datestring, sizeof(datestring),"%c", uptm);

  if(logRingIndex_ == 0){logWrap_ = true; logRingIndex_ = logRingSize_;}
  logRingIndex_--;
  std::ostringstream timestamp;
  timestamp << " at " << datestring;
  m += timestamp.str();
  logRing_[logRingIndex_] = m;
}

//helper stuff->will be moved somewhere
//______________________________________________________________________________
bool Processor::epStringIndexT::cacheNew(std::vector<char*> pieces, std::vector<unsigned> pieceSizes, int bsize) {

	if (!bsize || !pieces.size()) return false;
	cached=false;
	std::string buf;
	for(unsigned int j = 0; j < pieces.size(); j++)
		buf+=std::string(pieces[j],pieceSizes[j]);
	
	boost::char_separator<char> sep(";");
	boost::tokenizer<boost::char_separator<char> > tokens(buf, sep);
	for (boost::tokenizer<boost::char_separator<char> >::iterator tok_iter = tokens.begin();
		   tok_iter != tokens.end(); ++tok_iter){
		   holder.push_back(*tok_iter);
		   cached=true;
	}

	return cached;
}

XDAQ_INSTANTIATOR_IMPL(dqmevf::Processor)
