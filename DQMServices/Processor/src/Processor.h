#ifndef DQMEVF_PROCESSOR_H
#define DQMEVF_PROCESSOR_H

#include "StateMachine.h"
#include "Exception.h"

#include "MsgQueue.h"
#include "SubProcess.h"

#include "DataFormats/Provenance/interface/ModuleDescription.h"

#include "xdaq/Application.h"
#include "xdaq/NamespaceURI.h"

#include "xdata/String.h"
#include "xdata/Integer.h"
#include "xdata/Boolean.h"
#include "xdata/Vector.h"
#include "xdata/UnsignedInteger32.h"
#include "xdata/ActionListener.h"
#include "xdata/InfoSpaceFactory.h"

#include "xgi/Input.h"
#include "xgi/Output.h"

#include "FWEPWrapper.h"
#include "Web.h"

#include <sys/time.h>
#include <pthread.h>

#include <list>
#include <vector>
#include <map>
using namespace std;
namespace dqmevf
{

  /* used only by Processor */
  class Css
  {
    public:
      void css(xgi::Input  *in,
	       xgi::Output *out) throw (xgi::exception::Exception)
	{
	  out->getHTTPResponseHeader().addHeader("Content-Type", "text/css");
	  
	  *out << "body"                                   << endl;
	  *out << "{"                                      << endl;
	  *out << "background-color: white;"               << endl;
	  *out << "font-family: Arial;"                    << endl;
	  *out << "}"                                      << endl;
	  *out                                             << endl;
	  *out << "table.modules"                          << endl;
	  *out << "{"                                      << endl;
	  *out << "font-family: Arial;"                    << endl;
	  *out << "border: thin dotted;"                   << endl;
	  *out << "}"                                      << endl;
	  *out                                             << endl;
	  *out << "table.modules colgroup"                 << endl;
	  *out << "{"                                      << endl;
	  *out << "width: 30%;"                            << endl;
	  *out << "border: solid;"                         << endl;
	  *out << "}"                                      << endl;
	  *out                                             << endl;
	  *out << "table.modules th"                       << endl;
	  *out << "{"                                      << endl;
	  *out << "color: white;"                          << endl;
	  *out << "background-color: #63F;"                << endl;
	  *out << "}"                                      << endl;
	  *out << "table.modules tr.special"               << endl;
	  *out << "{"                                      << endl;
	  *out << "color: white;"                          << endl;
	  *out << "background-color: #000;"                << endl;
	  *out << "}"                                      << endl;
	  
	  *out << "table.states"                           << endl;
	  *out << "{"                                      << endl;
	  *out << "font-family: Arial;"                    << endl;
	  *out << "border: thin dotted;"                   << endl;
	  *out << "}"                                      << endl;
	  *out                                             << endl;
	  *out << "table.states th"                        << endl;
	  *out << "{"                                      << endl;
	  *out << "color: white;"                          << endl;
	  *out << "background-color: #63F;"                << endl;
	  *out << "}"                                      << endl;
	  *out << "table.states tr.special"                << endl;
	  *out << "{"                                      << endl;
	  *out << "color: white;"                          << endl;
	  *out << "background-color: #000;"                << endl;
	  *out << "}"                                      << endl;
	  *out                                             << endl;
	}
  };

  class Processor : public xdaq::Application,
			   public xdata::ActionListener
  {
  public:
    //
    // construction/destruction
    //
    XDAQ_INSTANTIATOR();
    Processor(xdaq::ApplicationStub *s);
    virtual ~Processor();
    

    //
    // member functions
    //

    // work loop functions to be executed during transitional states (async)
    bool configuring(toolbox::task::WorkLoop* wl);
    bool enabling(toolbox::task::WorkLoop* wl);
    bool halting(toolbox::task::WorkLoop* wl);
    bool shuttingdown(toolbox::task::WorkLoop* wl);

    // fsm soap command callback
    xoap::MessageReference fsmCallback(xoap::MessageReference msg)
      throw (xoap::exception::Exception);
        
    // xdata:ActionListener interface
    void actionPerformed(xdata::Event& e);
    
    // trigger report related helper functions
    //    std::string triggerReportToString(const edm::TriggerReport& tr);
    //    void triggerReportToTable(const edm::TriggerReport& tr);
    //    void        printTriggerReport(const edm::TriggerReport& tr);

    // HyperDAQ related functions
    void defaultWebPage(xgi::Input *in,xgi::Output *out)
      throw(xgi::exception::Exception);
    void spotlightWebPage(xgi::Input *in,xgi::Output *out)
      throw(xgi::exception::Exception);
    void css(xgi::Input *in,xgi::Output *out) throw (xgi::exception::Exception)
    {
      css_.css(in,out);
    }

    void subWeb(xgi::Input *in,xgi::Output *out);
    void moduleWeb(xgi::Input *in,xgi::Output *out);
    void serviceWeb(xgi::Input *in,xgi::Output *out);
    void microState(xgi::Input *in,xgi::Output *out);
    void updater(xgi::Input *in,xgi::Output *out);
    void procStat(xgi::Input *in,xgi::Output *out);
    void summaryWeb(xgi::Input *in, xgi::Output *out);
    void initiateReset(xgi::Input *in, xgi::Output *out);

    void manualConfigure(xgi::Input  *in, xgi::Output *out);
    void manualEnable(xgi::Input  *in, xgi::Output *out);
    void manualHalt(xgi::Input  *in, xgi::Output *out);
    void manualRecover(xgi::Input  *in, xgi::Output *out);

  private:

    void subWebRetriever(xgi::Output*, std::string, pid_t, int par1, std::ostringstream * cgiPassThrough);

    bool testEDMConfiguration();
    bool initEDMConfiguration();
    void reconfigureEP();
    void spawnChild(unsigned int);

    std::string logsAsString();
    void localLog(std::string);

    void startSupervisorWorkLoop();
    bool supervisor(toolbox::task::WorkLoop* wl);
    
    void startSummarizeWorkLoop() throw (dqmevf::Exception);
    bool summarize(toolbox::task::WorkLoop* wl);

    void stopSlavesAndAcknowledge();
    void makeStaticInfo();
    //
    // Class members
    //

    xdata::InfoSpace*                applicationInfoSpace_;
    dqmevf::StateMachine             fsm_;
    
    // logger
    Logger                           log_;

    // edm event processor wrapper
    FWEPWrapper                      fwepWrapper_;
    WrapperConfig                    wCfg_;
        
    xdata::UnsignedInteger32         runNumber_;
    xdata::String                    runType_;
    xdata::String                    runKey_;
    xdata::Boolean                   runTypeAuto_;
    xdata::Boolean                   autoRestartSlaves_;
    xdata::UnsignedInteger32         slaveRestartDelaySecs_;
    xdata::Boolean                   hasShMem_;
    xdata::Boolean                   hasModuleWebRegistry_;
    xdata::Boolean                   hasServiceWebRegistry_;
    xdata::Boolean                   exitOnError_;
    xdata::UnsignedInteger32         superSleepSec_;
    xdata::UnsignedInteger32         monSleepSec_;
    xdata::UnsignedInteger32         lsTimeOut_;
    xdata::UnsignedInteger32         idleRestartTime_;
    xdata::UnsignedInteger32         timeoutOnStop_;
    xdata::UnsignedInteger32         detachTimeout_;
    xdata::UnsignedInteger32         msgQueueOffset_;
    xdata::Boolean                   detachedRunning_;
    
    xdata::String                    configString_;
    xdata::String                    configStringCopy_;
    std::string                      friendlyPythonCfg_;
    xdata::UnsignedInteger32         instance_;
    //xdata::String                    logLevel_;    

   
    // HyperDAQ
    Css                              css_;

    //logging
    std::string                      reasonForFailedState_;
    std::vector<std::string>         logRing_;
    unsigned int                     logRingIndex_;
    static const unsigned int        logRingSize_ = 50;
    bool                             logWrap_;

    xdata::UnsignedInteger32         nbSubProcesses_;
    xdata::UnsignedInteger32         nbSubProcessesReporting_;
    std::vector<SubProcess>          subs_;
    std::vector<std::pair<int,int> > detached_;

    //trigger report stuff
    std::vector<unsigned int>         lsUpdated_;
    static const unsigned int         keepTrhHistory_ = 5;
    unsigned int                      lastReceivedTrLs_;
    unsigned int                      trhHistoryIndex_;
    std::vector<TriggerReportHelpers*> trhHistorySum_;
    std::vector<TriggerReportHelpers*> trh_;
    std::vector<TriggerReportHelpers*> trhAcc_;
    TriggerReportHelpers*             trhSumAcc_;
    unsigned int                      lastKnownLS_;

    //supervisor workloop
    toolbox::task::WorkLoop          *wlSupervising_;
    toolbox::task::ActionSignature   *asSupervisor_;
    bool                              supervising_;


    //summarize workloop
    toolbox::task::WorkLoop          *wlSummarize_;  
    toolbox::task::ActionSignature   *asSummarize_;
    bool                              wlSummarizeActive_;
   
    xdata::Boolean                    hasScalersService_;

    std::string                       updaterStatic_;
    
    pthread_mutex_t                   stop_lock_;
    pthread_mutex_t                   start_lock_;
    pthread_mutex_t                   start_lock2_;
    pthread_mutex_t                   pickup_lock_;
    pthread_mutex_t                   cfg_refresh_lock_;
   
    pthread_mutex_t                   sup_queue_lock_;
    pthread_mutex_t                   summary_queue_lock_;

    sem_t *                           semaphore_;
    //todo:useful?
    //xdata::Serializable             *nbProcessed;
    //xdata::Serializable             *nbAccepted;

    int                              anonymousPipe_[2];
    
    xdata::Vector<xdata::Integer>    spMStates_;
    xdata::Vector<xdata::Integer>    spmStates_;

    //flags 
    bool                             configurationInitialized_ ;
    bool                             configurationInit_ ;
    bool                             isStopping_;
    unsigned int                     inForcedKillMask_;
    bool                             debug_;

    Web *                            webHelper_;

   //objects used to hold simple info retrieved from child process
   struct masterStatT {
     masterStatT(){
       nbProcessed=0;
       nbAccepted=0;
       nbDQMProcessed=0;
       nbProcessedPrev=0;
       nbAcceptedPrev=0;
       nbDQMProcessedPrev=0;
     }
     void storeToPrev() {
       nbProcessedPrev+=nbProcessed;
       nbAcceptedPrev+=nbAccepted;
       nbDQMProcessedPrev=nbDQMProcessedPrev;
     }
     int nbProcessed;
     int nbAccepted;
     int nbDQMProcessed;
     int nbProcessedPrev;
     int nbAcceptedPrev;
     int nbDQMProcessedPrev;
   } masterStat_;

    struct epTagT {
	    bool cached;
	    std::string holder;
	    void reset() {cached=false;}
	    void fill (xgi::Output * out) {
		    *out << holder;
	    }
	    bool cacheNew(std::vector<char*> pieces, std::vector<unsigned> pieceSizes, int bcount) {
		    if (bcount==0 || !pieces.size()) return false;
		    holder = std::string();
		    for(unsigned int j = 0; j < pieces.size(); j++){
			    holder+=std::string(pieces[j],pieceSizes[j]);
		    }
		    cached=true;
		    return cached;
	    }

    };
    epTagT epModuleDescs_;
    
    struct epStringIndexT {
	    bool cached;
	    std::vector<std::string> holder;
	    void reset() {cached=false;holder.clear();}

	    std::string get(int idxAskedFor) {
		    if (idxAskedFor<0 || idxAskedFor>=(int)holder.size())
			    return std::string(" NA ");
		    else 
		      return holder[idxAskedFor];
	    }

	    void fill (xgi::Output *out, int idxAskedFor) {
		    if (idxAskedFor<0 || idxAskedFor>=(int)holder.size())
			    *out << " NA ";
		    else
		    *out << holder[idxAskedFor];
	    }
	    bool cacheNew(std::vector<char*> pieces, std::vector<unsigned> pieceSizes, int bcount);
	    
    };
    epStringIndexT  epModuleIndex_;
    epStringIndexT  epStateIndex_;

  };//end FUEP
 


} // namespace dqmevf


#endif
