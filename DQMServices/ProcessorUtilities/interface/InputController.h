#ifndef EVF_INPUTCONTROLLER_H
#define EVF_INPUTCONTROLLER_H


//#include "toolbox/lang/Class.h"
#include <string>


namespace dqmevf
{

  //used to transfer information to the input source
  struct evfSourceVars {

    unsigned int evtLumi;
    unsigned int lumi;
    unsigned int lastLumi;
	unsigned int processedEvents;
	unsigned int processedDQMEvents;
	unsigned int droppedEvents;
	unsigned int droppedDQMEvents;
    unsigned int nbSubProcesses;
    unsigned int trappingCounter;
    bool hasTimedOut;
    bool enableLocking;
    bool dropOldLS;
    bool isDQM;
    volatile bool *runStopFlag;

    evfSourceVars() {
      evtLumi=0;
      lumi=0;
      lastLumi=0;
      processedEvents=0;
      droppedEvents=0;
      processedDQMEvents=0;
      droppedDQMEvents=0;
      nbSubProcesses=1;
      trappingCounter=1;//will be decreased on init
      hasTimedOut=false;
      enableLocking=true;
      dropOldLS=true;
      isDQM=false;
      runStopFlag=0;
    }
  };

  class InputController
  {
    public:
      InputController(const std::string &);
      virtual ~InputController(){}
      virtual void publish(evfSourceVars * varsPtr) = 0;
    protected:
      std::string name_;
    private:
      virtual void trapSource(unsigned int timeout_sec = 0){};
      virtual void untrapSource(){};
      friend class InputControllerRegistry;
  };
}
#endif
