#ifndef FilterUnitFramework_H
#define FilterUnitFramework_H
//
///
//
//
/// Author: E.Meschi PH/CMD
///
//
//  Modification history:
//    $Log: FilterUnitFramework.h,v $
//    Revision 1.12  2006/10/02 08:59:15  meschi
//    added variables for flashlists
//
//    Revision 1.11  2006/09/26 16:26:01  schiefer
//    ready for xdaq 3.7 / CMSSW_1_1_0
//
//    Revision 1.10  2006/09/21 12:39:44  schiefer
//    fix crc check performed in FURawEvent
//
//    Revision 1.9  2006/09/04 15:00:10  schiefer
//    FUAdapter::doCrcCheck_=n indicates now that every n-th event will have its crc values checked, not every n-th superfragment
//
//    Revision 1.8  2006/06/13 15:09:56  meschi
//    modifications to signal a thread waiting on input
//
//    Revision 1.7  2006/05/23 11:43:49  meschi
//    additional diagnostics
//
//    Revision 1.6  2006/05/18 13:35:18  vuko
//    adding XDAQ monitoring to FU
//
//    Revision 1.5  2006/02/15 09:22:34  meschi
//    changes to reflect EPStateMachine changes
//
//    Revision 1.4  2005/11/28 11:39:14  meschi
//    Added debug web with fragment dump, added workdir configurable
//
//    Revision 1.3  2005/11/10 14:41:16  meschi
//    run number now in Utilities/RunBase
//
//    Revision 1.2  2005/10/20 11:52:48  meschi
//    use of Utilities
//
//    Revision 1.1  2005/10/19 09:10:35  meschi
//    first import from COSINE
//
//    Revision 1.9  2005/10/07 15:32:33  meschi
//    flush logic
//
//    Revision 1.8  2005/07/15 07:47:39  meschi
//    correct run initialization and dataset management
//
//    Revision 1.7  2005/06/28 08:49:02  meschi
//    added support for orcarc upload
//
//    Revision 1.6  2005/06/17 12:20:28  meschi
//    added state name and collector config
//
//    Revision 1.5  2005/02/23 09:17:19  meschi
//    XDAQ3 Porting
//
//    Revision 1.4  2005/01/06 11:30:58  meschi
//    switchable debug printout
//
//    Revision 1.3  2004/09/22 09:47:28  meschi
//    mods for evb integration debugging
//
//    Revision 1.2  2004/07/06 08:18:31  meschi
//    more thread handling
//
//    Revision 1.1  2004/06/30 22:16:35  meschi
//    now compatible with cobra task and daqevent
//
//
//
#if defined(linux) || defined(macosx)
using namespace std;
#endif

#include <semaphore.h>
#include <map>
#include <vector>
#include <deque>
#include <pthread.h>

#include "toolbox/include/toolbox/task/TimerFactory.h"
#include "toolbox/include/Task.h"
#include "toolbox/include/TaskGroup.h"
#include "toolbox/include/toolbox/Chrono.h"
#include "xdata/include/xdata/UnsignedInteger32.h"
#include "xoap/include/xoap/SOAPEnvelope.h"
#include "xdata/include/xdata/Float.h"
#include "xdata/include/xdata/Double.h"
#include "xdata/include/xdata/String.h"
#include "xoap/include/xoap/SOAPBody.h"
#include "xoap/include/xoap/domutils.h"
#include "xgi/include/xgi/Input.h"
#include "xgi/include/xgi/Output.h"
#include "xgi/include/xgi/exception/Exception.h"
#include "xdaq/include/xdaq/NamespaceURI.h"

#include "EventFilter/Unit/interface/FUAdapter.h"
#include "EventFilter/Utilities/interface/EPStateMachine.h"
#include "EventFilter/Utilities/interface/Css.h"


//
// forward-declarations


class FilterUnitFramework : public FUAdapter,
			    public toolbox::task::TimerListener,
			    public xdata::ActionListener/*,
			    public soapMonitorAdapter,
			    public soapConfigurationListener,
			    public xdaqApplication */
{
public:

  //macro used by xdaq to register the plugin hook
  XDAQ_INSTANTIATOR();

  // constructor
  FilterUnitFramework(xdaq::ApplicationStub *s);

  virtual ~FilterUnitFramework();

  // The main interface for the HLT
  FURawEvent * rqstEvent();

  void signalWaitingInput();

  unsigned long myId(){return instance_;}
  double birth(){return birth_.tstamp();}

  //monitoring infospace
  xdata::InfoSpace *s_mon;

private:

  toolbox::task::Timer * Monitor_timer_;
  void timeExpired (toolbox::task::TimerEvent& e);
  xdata::Boolean MonitorTimerEnable_;
  xdata::Double MonitorIntervalSec_;
  xdata::Double MonitorEventsPerSec_;

  // config exec

  xdata::UnsignedInteger32 buInstance_;

  // config Fw

  xdata::UnsignedInteger32 queueSize_;

  // config hlt

  xdata::Boolean runActive_;
  xdata::String  workDir_;

  // exported (monitor) variables

  xdata::UnsignedInteger32 nbEvents_;
  xdata::UnsignedInteger32 nbProcessedEvents_;
  xdata::String class_;
  xdata::UnsignedInteger32 instance_;
  xdata::String url_;
  xdata::String host_;

  // stats

  toolbox::Chrono birth_;
  double deltaT;
  vector<int> runs;
  vector<int> events;
  // locks
  BSem *mutex_;
  BSem *ack_;
  sem_t empty_; // a POSIX counting semaphore
  BSem *comm_;
  pthread_cond_t ready_;
  pthread_mutex_t lock_;

  //flags
  bool flush_;

  // components (see inheritance tree for more components
  // pool of filter tasks

  TaskGroup             *taskGroup_;

  // private service methods

  void exportParams();

  /**
   * State Machine and related methods
   */
  evf::EPStateMachine *fsm_;

  /**
   * Non trivial state-change actions
   */

  void configureAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
  void enableAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
  virtual void suspendAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
  virtual void resumeAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
  virtual void haltAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
  virtual void nullAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);

  /**
   * Web interface member data and functions
   */
  evf::Css css_;
  void defaultWebPage(xgi::Input *in, xgi::Output *out) throw (xgi::exception::Exception);
  void debugWebPage(xgi::Input *in, xgi::Output *out) throw (xgi::exception::Exception);
  unsigned char *dump_;
  int fed_id;
  void parameterTables(xgi::Input *in, xgi::Output *out);
  void css(xgi::Input  *in,
	   xgi::Output *out) throw (xgi::exception::Exception)
  {css_.css(in,out);}  
  xoap::MessageReference getStateMsg(xoap::MessageReference msg) throw (xoap::exception::Exception);
  
  /**
   * SOAP Callback used to trigger state change.
   */
  xoap::MessageReference fireEvent(xoap::MessageReference msg)
    throw (xoap::exception::Exception)
    {
      xoap::SOAPPart     part      = msg->getSOAPPart();
      xoap::SOAPEnvelope env       = part.getEnvelope();
      xoap::SOAPBody     body      = env.getBody();
      DOMNode            *node     = body.getDOMNode();
      DOMNodeList        *bodyList = node->getChildNodes();
      DOMNode            *command  = 0;
      std::string        commandName;
      
      for (unsigned int i = 0; i < bodyList->getLength(); i++)
	{
	  command = bodyList->item(i);
	  
	  if(command->getNodeType() == DOMNode::ELEMENT_NODE)
            {
	      commandName = xoap::XMLCh2String(command->getLocalName());
	      return fsm_->processFSMCommand(commandName);
            }
        }
      
      XCEPT_RAISE(xoap::exception::Exception, "Command not found");
    }

  /**
   * Processes FSM commands from run-control.
   */
  //now delegated to EPStateMachine;
  friend class evf::EPStateMachine;

  /** default actions for commands not yet implemented */

  virtual void disableAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception) {}
  
  virtual void failAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception) {}


  /** miscellaneous actions */

  void flushBuiltQueue();

  void actionPerformed (xdata::Event& e);

  void resetCounters();
};

#endif
