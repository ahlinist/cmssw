#ifndef DQMEVF_STATEMACHINE_H
#define DQMEVF_STATEMACHINE_H


#include "xdaq/Application.h"
#include "xdaq/NamespaceURI.h"

#include "toolbox/fsm/FiniteStateMachine.h"
#include "toolbox/task/WorkLoopFactory.h"
#include "toolbox/task/Action.h"

#include "xoap/MessageReference.h"
#include "xoap/MessageFactory.h"
#include "xoap/Method.h"

#include "xdata/String.h"
#include "xdata/Bag.h"

#include "xdaq2rc/ClassnameAndInstance.h"
#include "xdaq2rc/RcmsStateNotifier.h"

#include <string>


namespace dqmevf
{
  
  class StateMachine : public toolbox::lang::Class
  {
  public:
    //
    // construction / destruction
    //
    StateMachine(xdaq::Application* app);
    virtual ~StateMachine();
    
    
    //
    // member functions 
    //
    
    // finite state machine command callback
    xoap::MessageReference commandCallback(xoap::MessageReference msg)
      throw (xoap::exception::Exception);
    
    // finite state machine callback for entering new state
    void stateChanged(toolbox::fsm::FiniteStateMachine & fsm) 
      throw (toolbox::fsm::exception::Exception);
    
    // finite state machine callback for transition into 'Failed' state
    void failed(toolbox::Event::Reference e)
      throw(toolbox::fsm::exception::Exception);
    
    // fire state transition event
    void fireEvent(const std::string& evtType,void* originator);
    
    // initiate transition to state 'Failed'
    void fireFailed(const std::string& errorMsg,void* originator);
    
    // find RCMS state listener
    void findRcmsStateListener();

    // disable rcms state notification
    void disableRcmsStateNotification(){doStateNotification_=false;}

    bool checkIfEnabled() {return fsm_.getCurrentState()=='E';}
    
    // report current state
    xdata::String* stateName() { return &stateName_; }
    
    // get the RCMS StateListener parameter (classname/instance)
    xdata::Bag<xdaq2rc::ClassnameAndInstance>* rcmsStateListener()
    {
      return rcmsStateNotifier_.getRcmsStateListenerParameter();
    }

    // report if RCMS StateListener was found
    xdata::Boolean* foundRcmsStateListener()
    {
      return rcmsStateNotifier_.getFoundRcmsStateListenerParameter();
    }

    // initialize state machine and bind callbacks to driving application
    template<class T> void initialize(T *app)
    {
      // action signatures
      asConfiguring_ = toolbox::task::bind(app,&T::configuring,"configuring");
      asEnabling_    = toolbox::task::bind(app,&T::enabling,   "enabling");
      asHalting_     = toolbox::task::bind(app,&T::halting,    "halting");
      asShuttingDown_     = toolbox::task::bind(app,&T::shuttingdown,    "shuttingDown");
      
      // work loops
      workLoopConfiguring_ =
	toolbox::task::getWorkLoopFactory()->getWorkLoop(appNameAndInstance_+
							 "_Configuring",
							 "waiting");
      workLoopEnabling_ =
	toolbox::task::getWorkLoopFactory()->getWorkLoop(appNameAndInstance_+
							 "_Enabling",
							 "waiting");
      workLoopHalting_ =
	toolbox::task::getWorkLoopFactory()->getWorkLoop(appNameAndInstance_+
							 "_Halting",
							 "waiting");
      workLoopShuttingDown_ =
        toolbox::task::getWorkLoopFactory()->getWorkLoop(appNameAndInstance_+
                                                         "_ShuttingDown",
                                                         "waiting");
 
      
      // bind SOAP callbacks
      xoap::bind(app,&T::fsmCallback,"Configure",XDAQ_NS_URI);
      xoap::bind(app,&T::fsmCallback,"Enable",   XDAQ_NS_URI);
      xoap::bind(app,&T::fsmCallback,"Halt",     XDAQ_NS_URI);
      xoap::bind(app,&T::fsmCallback,"Shutdown",     XDAQ_NS_URI);
      
      // define finite state machine, states&transitions
      fsm_.addState('h', "halting"    , this,&dqmevf::StateMachine::stateChanged);
      fsm_.addState('H', "Halted"     , this,&dqmevf::StateMachine::stateChanged);
      fsm_.addState('c', "configuring", this,&dqmevf::StateMachine::stateChanged);
      fsm_.addState('R', "Ready"      , this,&dqmevf::StateMachine::stateChanged);
      fsm_.addState('e', "enabling"   , this,&dqmevf::StateMachine::stateChanged);
      fsm_.addState('E', "Enabled"    , this,&dqmevf::StateMachine::stateChanged);
      fsm_.addState('X', "Degraded"    , this,&dqmevf::StateMachine::stateChanged);
      fsm_.addState('s', "shuttingDown",this,&dqmevf::StateMachine::stateChanged);
      fsm_.addState('S', "ShutDown",    this,&dqmevf::StateMachine::stateChanged);
      
      fsm_.addStateTransition('H','c',"Configure");
      fsm_.addStateTransition('c','R',"ConfigureDone");
      fsm_.addStateTransition('R','e',"Enable");
      fsm_.addStateTransition('e','E',"EnableDone");
      fsm_.addStateTransition('X','E',"EnableDone");
      fsm_.addStateTransition('E','X',"Degrade");
      fsm_.addStateTransition('E','h',"Halt");
      fsm_.addStateTransition('X','h',"Halt");
      fsm_.addStateTransition('R','h',"Halt");
      fsm_.addStateTransition('h','H',"HaltDone");
      fsm_.addStateTransition('e','H',"EnableAbort");

      //shutdown(from any state)
      fsm_.addStateTransition('R','s',"Shutdown");
      fsm_.addStateTransition('c','s',"Shutdown");
      fsm_.addStateTransition('E','s',"Shutdown");
      fsm_.addStateTransition('X','s',"Shutdown");
      fsm_.addStateTransition('e','s',"Shutdown");
      fsm_.addStateTransition('H','s',"Shutdown");
      fsm_.addStateTransition('h','s',"Shutdown");
      fsm_.addStateTransition('F','s',"Shutdown");
      fsm_.addStateTransition('s','S',"ShutdownDone");

      fsm_.addStateTransition('c','F',"Fail",this,&dqmevf::StateMachine::failed);
      fsm_.addStateTransition('e','F',"Fail",this,&dqmevf::StateMachine::failed);
      fsm_.addStateTransition('h','F',"Fail",this,&dqmevf::StateMachine::failed);
      
      fsm_.addStateTransition('E','F',"Fail",this,&dqmevf::StateMachine::failed);
      fsm_.addStateTransition('X','F',"Fail",this,&dqmevf::StateMachine::failed);

      fsm_.setFailedStateTransitionAction(this,&dqmevf::StateMachine::failed);
      fsm_.setFailedStateTransitionChanged(this,&dqmevf::StateMachine::stateChanged);
      fsm_.setStateName('F',"Failed");
      
      fsm_.setInitialState('H');
      fsm_.reset();
      stateName_ = fsm_.getStateName(fsm_.getCurrentState());
      
      if (!workLoopConfiguring_->isActive())  workLoopConfiguring_  ->activate();
      if (!workLoopEnabling_->isActive())     workLoopEnabling_     ->activate();
      if (!workLoopHalting_->isActive())      workLoopHalting_      ->activate();
      if (!workLoopShuttingDown_->isActive()) workLoopShuttingDown_ ->activate();
    }
    
     
  private:
    //
    // member data
    //
    
    // application name&instance
    log4cplus::Logger                logger_;
    xdata::InfoSpace*                appInfoSpace_;
    std::string                      appNameAndInstance_;
    xdata::String                    stateName_;
    bool                             doStateNotification_;
    
    // finite state machine
    toolbox::fsm::FiniteStateMachine fsm_;
    
    // work loops for transitional states
    toolbox::task::WorkLoop         *workLoopConfiguring_;
    toolbox::task::WorkLoop         *workLoopEnabling_;
    toolbox::task::WorkLoop         *workLoopHalting_;
    toolbox::task::WorkLoop         *workLoopShuttingDown_;

    // action signatures for transitional states
    toolbox::task::ActionSignature  *asConfiguring_;
    toolbox::task::ActionSignature  *asEnabling_;
    toolbox::task::ActionSignature  *asHalting_;
    toolbox::task::ActionSignature  *asShuttingDown_;
    
    // rcms state notifier
    xdaq2rc::RcmsStateNotifier       rcmsStateNotifier_;
    
  };
  
}


#endif
