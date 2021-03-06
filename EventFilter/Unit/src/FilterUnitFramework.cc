//
/// See header file for a description of this class
//
//
/// Author: E.Meschi PH/CMD
///
//
//  Modification history:
//    $Log: FilterUnitFramework.cc,v $
//    Revision 1.23  2007/02/14 09:48:03  meschi
//    removed using directive, added stopAction
//
//    Revision 1.22  2006/11/14 10:10:02  meschi
//    fix bug for queueSize = 1, added bu range
//
//    Revision 1.21  2006/11/11 12:28:08  schiefer
//    tag V00-11-01: removed extra ';' for slc4/gcc3.4.5 compliance
//
//    Revision 1.20  2006/10/31 21:07:25  meschi
//     fix to timer call in rate configure and reset unused resources upon halting
//
//    Revision 1.19  2006/10/19 12:29:38  schiefer
//    fix to crc error counter web display
//
//    Revision 1.18  2006/10/05 08:46:09  meschi
//    fixes for reconfigure upon halt, implementation of flush for halt, reset of counters upon reception of new run number
//
//    Revision 1.17  2006/10/02 09:17:51  meschi
//    mods for FU flashlist
//
//    Revision 1.16  2006/09/26 16:26:01  schiefer
//    ready for xdaq 3.7 / CMSSW_1_1_0
//
//    Revision 1.15  2006/09/21 12:39:44  schiefer
//    fix crc check performed in FURawEvent
//
//    Revision 1.14  2006/09/04 15:00:10  schiefer
//    FUAdapter::doCrcCheck_=n indicates now that every n-th event will have its crc values checked, not every n-th superfragment
//
//    Revision 1.13  2006/08/30 08:45:20  schiefer
//    added 'doCrcCheck' paramete rand 'nbCrcErros' counter, removed cout statements
//
//    Revision 1.12  2006/08/29 16:55:43  schiefer
//    temporary fix to FURawEvent::processMsg: return value indicates now super fragment number eventhough error has occured
//
//    Revision 1.11  2006/08/08 11:20:32  meschi
//    reset pending requests when halting
//
//    Revision 1.10  2006/06/13 15:10:42  meschi
//    modifications to signal a thread waiting on input
//
//    Revision 1.9  2006/05/23 11:45:13  meschi
//    additional diagnostics in main and debug web, removed dqm-related stuff
//
//    Revision 1.8  2006/05/18 13:35:19  vuko
//    adding XDAQ monitoring to FU
//
//    Revision 1.7  2006/02/15 09:22:34  meschi
//    changes to reflect EPStateMachine changes
//
//    Revision 1.6  2006/02/13 09:41:42  meschi
//    capture chdir error setting working directory
//
//    Revision 1.5  2005/11/28 11:40:11  meschi
//    Added debug web with fragment dump, added workdir configurable
//
//    Revision 1.3  2005/11/10 14:44:54  meschi
//    simplified synchro out of COBRA
//
//    Revision 1.2  2005/10/20 11:52:58  meschi
//    use of Utilities
//
//    Revision 1.1  2005/10/19 09:10:35  meschi
//    first import from COSINE
//
//    Revision 1.14  2005/10/07 15:35:51  meschi
//    flush logic
//
//    Revision 1.13  2005/07/15 07:47:55  meschi
//    correct run initialization and dataset management
//
//    Revision 1.12  2005/06/28 09:09:06  meschi
//    added check if orcarc config is empty
//
//    Revision 1.11  2005/06/28 08:55:25  meschi
//    improved run logic and orcarc upload
//
//    Revision 1.10  2005/06/17 12:32:43  meschi
//    added stateName and update, monitor config, and task page
//
//    Revision 1.9  2005/06/15 10:54:45  namapane
//    fix html header
//
//    Revision 1.8  2005/05/04 07:08:50  meschi
//    set the working directory (from PWD) in configure
//
//    Revision 1.7  2005/02/28 09:35:38  meschi
//    porting to XDAQ3
//
//    Revision 1.6  2005/02/23 09:17:54  meschi
//    XDAQ3 Porting
//
//    Revision 1.5  2005/01/06 11:48:48  meschi
//    switchable debug printout
//
//    Revision 1.4  2005/01/06 11:15:43  meschi
//    fix for halt of evb ???
//
//    Revision 1.3  2004/09/22 09:47:28  meschi
//    mods for evb integration debugging
//
//    Revision 1.2  2004/07/05 11:08:48  meschi
//    halt shuts down cobra
//
//    Revision 1.1  2004/06/30 22:17:32  meschi
//    now compatible with cobra task and daqevent
//
//
//

#include "toolbox/Chrono.h"
#include "xcept/tools.h"
#include "xoap/MessageFactory.h"
#include "xoap/Method.h"
#include "xgi/Method.h"

#include "EventFilter/Unit/interface/FilterUnitFramework.h"
#include "EventFilter/Unit/interface/FURawEventFactory.h"

#include "EventFilter/Utilities/interface/DebugUtils.h"
#include "EventFilter/Unit/interface/FURawEvent.h"

#include "i2o/utils/AddressMap.h"
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc//FormEntry.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include <stdio.h>

#include <typeinfo>
#include <unistd.h>
#include <sys/utsname.h>

#include <pthread.h>
#include <sys/types.h>
#include <time.h>

#include <signal.h>


using namespace std;

FilterUnitFramework::FilterUnitFramework(xdaq::ApplicationStub *s) : FUAdapter(s),
								     nbEvents_(0),
								     flush_(false),
								     fsm_(0)
{
  cout << "Entered constructor " << endl;
  Monitor_timer_=0;  
  mutex_ = new toolbox::BSem(toolbox::BSem::FULL);
  
  fsm_ = new evf::EPStateMachine(getApplicationLogger());
  fsm_->init<FilterUnitFramework>(this);
  
  xoap::bind(this, &FilterUnitFramework::getStateMsg,"getState",XDAQ_NS_URI);

  exportParams();

  // Bind web interface
   xgi::bind(this, &FilterUnitFramework::css           , "styles.css");
   xgi::bind(this, &FilterUnitFramework::defaultWebPage, "Default");
   xgi::bind(this, &FilterUnitFramework::debugWebPage, "debug");

  LOG4CPLUS_INFO(this->getApplicationLogger(),
		 xmlClass_ << instance_ << " constructor");

  cout << xmlClass_ << instance_ << " constructor" << endl;
  pthread_mutex_init(&lock_,0);
  pthread_cond_init(&ready_,0);
  dump_ = new unsigned char[128000];
  fed_id = -1;
  
}


void FilterUnitFramework::exportParams()
{
  xdata::InfoSpace *s = getApplicationInfoSpace();
  // default configuration
  buInstance_     = 0;
  buMaxInstance_  = 0;
  bui_ = 0;
  queueSize_ = 16;

  class_ = getApplicationDescriptor()->getClassName();
  instance_ = getApplicationDescriptor()->getInstance();
  std::string surl;
  surl =  getApplicationDescriptor()->getContextDescriptor()->getURL();
  host_ = surl;
  surl += "/";
  surl +=  getApplicationDescriptor()->getURN();
  url_=surl;

  runActive_ = false;
  workDir_ = "/tmp/evf";
  runNumber_ = 1;
  MonitorTimerEnable_=true;
  MonitorIntervalSec_=1.0;
  MonitorEventsPerSec_=0.0;
  s->fireItemAvailable("buInstance",&buInstance_);
  s->fireItemAvailable("buMaxInstance",&buMaxInstance_);

  // additional configuration specific to parametric FU

  s->fireItemAvailable("queueSize",&queueSize_);
  s->fireItemAvailable("stateName",&fsm_->stateName_);
  s->fireItemAvailable("runActive",&runActive_);
  s->fireItemAvailable("runNumber",&runNumber_);
  s->fireItemAvailable("workDir",&workDir_);
  
  s->fireItemAvailable("MonitorTimerEnable",&MonitorTimerEnable_);
  s->fireItemAvailable("MonitorIntervalSec",&MonitorIntervalSec_);
  
  //Monitoring infospace and variables

  nbProcessedEvents_ = factory_->getnbProcessed();

  std::ostringstream oss;
  oss << "urn:xdaq-monitorable:" << class_.value_ << ":" << instance_.value_;
  s_mon = xdata::InfoSpace::get(oss.str());
  s_mon->fireItemAvailable("class",&class_);
  s_mon->fireItemAvailable("hostname",&host_);
  s_mon->fireItemAvailable("instance",&instance_);
  s_mon->fireItemAvailable("url",&url_);
  s_mon->fireItemAvailable("stateName",&fsm_->stateName_);
  s_mon->fireItemAvailable("runNumber",&runNumber_);
  s_mon->fireItemAvailable("crcCheckEv",&doCrcCheck_);
  s_mon->fireItemAvailable("nbReceivedEvents",&nbReceivedEvents_);
  s_mon->fireItemAvailable("nbProcessedEvents",&nbProcessedEvents_);
  s_mon->fireItemAvailable("nbReceivedFragments",&nbReceivedFragments_);
  s_mon->fireItemAvailable("pendingRequests",&pendingRequests_);
  s_mon->fireItemAvailable("nbDataErrors",&nbDataErrors_);
  s_mon->fireItemAvailable("nbCrcErrors",&nbCrcErrors_);
  s_mon->fireItemAvailable("eventRate",&MonitorEventsPerSec_);

  // Add infospace listeners for exported parameters watch
  getApplicationInfoSpace()->addItemChangedListener ("runNumber", this);


}

FilterUnitFramework::~FilterUnitFramework()
{
  xdata::InfoSpace::remove("urn:xdaq-monitorable:FUFramework");
  delete factory_;
  delete dump_;
}


void FilterUnitFramework::configureAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  string reason;
  if(!checkParameters(reason))
    XCEPT_RAISE (toolbox::fsm::exception::Exception, 
		 reason);
  flush_ = false;
  if(!runActive_) findOrCreateMemoryPool();
  const char *workdir = 0;
  int retval = 0;
  if(workDir_.value_=="")
    workdir = getenv("PWD");
  else
    workdir = workDir_.value_.c_str();
  if(workdir != 0)
    retval = chdir(workdir);
  if(retval==0)
    {
      LOG4CPLUS_DEBUG(this->getApplicationLogger(),"FU Working Directory set to "
		     << workdir);
    }
  else
    {
      LOG4CPLUS_DEBUG(this->getApplicationLogger(),"could not set working directory to" << workdir);
    }
  if(!runActive_) createBUArray();

  //setup monitoring timer
  if (MonitorTimerEnable_ && !runActive_) {
  	toolbox::task::getTimerFactory()->createTimer("monitoringTask");
  	Monitor_timer_=toolbox::task::getTimerFactory()->getTimer("monitoringTask");
  	Monitor_timer_->stop();
  }
  factory_->resetNbProcessed();
}

void FilterUnitFramework::enableAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  cout<<"FilterUnitFramework::Enable()"<<endl;
  runActive_ = true;
  birth_.start(0); // start timer before sending allocate

  //
  // Start operation by requesting the first queueSize_ events
  //
  // (Fill in the event queue)

  unsigned int allocfset = 1;
  LOG4CPLUS_INFO(this->getApplicationLogger(),"FU sends initial allocate N=" << queueSize_);
  bui_ = buInstance_;
  sendAllocate(buInstance_, queueSize_, allocfset);
  nbEvents_ = 0;
  //start monitoring timer
  LOG4CPLUS_INFO(this->getApplicationLogger(),"FU sent initial allocate N=" << queueSize_);
  if (MonitorTimerEnable_ && !runActive_) {
  	toolbox::TimeInterval interval(MonitorIntervalSec_.value_);
  	toolbox::TimeVal startTime;
  	startTime = toolbox::TimeVal::gettimeofday();
  	Monitor_timer_->start();
  	Monitor_timer_->scheduleAtFixedRate(startTime, this, interval, s_mon, "monitoringTask" );
  }

  LOG4CPLUS_INFO(this->getApplicationLogger(),"Run Started...");
}

void FilterUnitFramework::stopAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  //  dummy for the moment
}


void FilterUnitFramework::suspendAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  //  pthread_t ttid = pthread_self();

  mutex_->take();
  LOG4CPLUS_INFO(this->getApplicationLogger(),"Run Paused...");
}

void FilterUnitFramework::resumeAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  //  pthread_t ttid = pthread_self();

  mutex_->give();
  LOG4CPLUS_INFO(this->getApplicationLogger(),"Run Resumed...");
}

void FilterUnitFramework::haltAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  flush_ = true;
  //  if(Monitor_timer_)Monitor_timer_->stop();
  int savePending = pendingRequests_;
  mutex_->take(); //exclude EP requests while flushing
  if(factory_->queueSize() != 0)
    {
      LOG4CPLUS_WARN(this->getApplicationLogger(),
		     "Built queue not empty. " << factory_->queueSize()
		     << " events are being discarded ");
      flushBuiltQueue();
      if(savePending != pendingRequests_)
	{
	  LOG4CPLUS_ERROR(this->getApplicationLogger(),
			  "Requests serviced during flush: " << savePending- pendingRequests_
			  << " this should never happen!!! ");
      	  //wait 50 ms then check again queue size 
	  usleep(50000);
	  flushBuiltQueue();
	}
    }
  mutex_->give();
  if(pendingRequests_ != 0)
    {
      LOG4CPLUS_WARN(this->getApplicationLogger(),
		     "Requests still pending after halt: " << pendingRequests_
		     << " will be forgotten ");
      pendingRequests_ = 0;
      factory_->resetFreeRes();
    }
}

void FilterUnitFramework::nullAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  //this action has no effect. A warning is issued to this end
  LOG4CPLUS_WARN(this->getApplicationLogger(),
		    "Null action invoked");
}

xoap::MessageReference FilterUnitFramework::getStateMsg(xoap::MessageReference msg)
  throw (xoap::exception::Exception)
{
  cout << "Creating getStateMsg response\n" << flush;
  xoap::MessageReference responseMsg = xoap::createMessage();
  xoap::SOAPEnvelope envelope = responseMsg->getSOAPPart().getEnvelope();
  xoap::SOAPBody body = envelope.getBody();
  xoap::SOAPName responseName =
    envelope.createName("getStateResponse","rcms","urn:rcms-soap:2.0");
  xoap::SOAPBodyElement responseElement =
    body.addBodyElement(responseName);
  responseElement.addNamespaceDeclaration("xsi",
					  "http://www.w3.org/2001/XMLSchema-instance");
  responseElement.addNamespaceDeclaration("xsd",
					  "http://www.w3.org/2001/XMLSchema");
  responseElement.addNamespaceDeclaration("soapenc",
					  "http://schemas.xmlsoap.org/soap/encoding/");
  xoap::SOAPName stateName =
    envelope.createName("state","rcms","urn:rcms-soap:2.0");
  xoap::SOAPElement stateElement =
    responseElement.addChildElement(stateName);
  xoap::SOAPName stateTypeName = envelope.createName("type", "xsi",
						     "http://www.w3.org/2001/XMLSchema-instance");
  stateElement.addAttribute(stateTypeName, "xsd:string");
  stateElement.addTextNode(fsm_->stateName_);

  return responseMsg;
}




void FilterUnitFramework::defaultWebPage(xgi::Input *in, xgi::Output *out)
  throw (xgi::exception::Exception)
{

  *out << "<html>"                                                   << endl;
  *out << "<head>"                                                   << endl;
  *out << "<link type=\"text/css\" rel=\"stylesheet\"";
  *out << " href=\"/" <<  getApplicationDescriptor()->getURN()
       << "/styles.css\"/>"                                          << endl;
  *out << "<title>" << getApplicationDescriptor()->getClassName()
       << getApplicationDescriptor()->getInstance()
       << " MAIN</title>"                                            << endl;
  *out << "</head>"                                                  << endl;
  *out << "<body>"                                                   << endl;
  *out << "<table border=\"0\" width=\"100%\">"                      << endl;
  *out << "<tr>"                                                     << endl;
  *out << "  <td align=\"left\">"                                    << endl;
  *out << "    <img"                                                 << endl;
  *out << "     align=\"middle\""                                    << endl;
  *out << "     src=\"/daq/evb/examples/fu/images/fu64x64.gif\""     << endl;
  *out << "     alt=\"main\""                                        << endl;
  *out << "     width=\"64\""                                        << endl;
  *out << "     height=\"64\""                                       << endl;
  *out << "     border=\"\"/>"                                       << endl;
  *out << "    <b>"                                                  << endl;
  *out << getApplicationDescriptor()->getClassName()
       << getApplicationDescriptor()->getInstance()                  << endl;
  *out << "      " << fsm_->stateName_.toString()                    << endl;
  *out << "    </b>"                                                 << endl;
  *out << "  </td>"                                                  << endl;
  *out << "  <td width=\"32\">"                                      << endl;
  *out << "    <a href=\"/urn:xdaq-application:lid=3\">"             << endl;
  *out << "      <img"                                               << endl;
  *out << "       align=\"middle\""                                  << endl;
  *out << "       src=\"/daq/xdaq/hyperdaq/images/HyperDAQ.jpg\""    << endl;
  *out << "       alt=\"HyperDAQ\""                                  << endl;
  *out << "       width=\"32\""                                      << endl;
  *out << "       height=\"32\""                                      << endl;
  *out << "       border=\"\"/>"                                     << endl;
  *out << "    </a>"                                                 << endl;
  *out << "  </td>"                                                  << endl;
  *out << "  <td width=\"32\">"                                      << endl;
  *out << "  </td>"                                                  << endl;
  *out << "  <td width=\"32\">"                                      << endl;
  *out << "    <a href=\"/" << getApplicationDescriptor()->getURN()
       << "/debug\">"                   << endl;
  *out << "      <img"                                               << endl;
  *out << "       align=\"middle\""                                  << endl;
  *out << "       src=\"/daq/evb/bu/images/debug32x32.gif\""         << endl;
  *out << "       alt=\"debug\""                                     << endl;
  *out << "       width=\"32\""                                      << endl;
  *out << "       height=\"32\""                                     << endl;
  *out << "       border=\"\"/>"                                     << endl;
  *out << "    </a>"                                                 << endl;
  *out << "  </td>"                                                  << endl;
  *out << "</tr>"                                                    << endl;
  *out << "</table>"                                                 << endl;

  *out << "<hr/>"                                                    << endl;
  *out << "<table>"                                                  << endl;
  *out << "<tr valign=\"top\">"                                      << endl;
  *out << "  <td>"                                                   << endl;
  parameterTables(in,out); //fill tables
  *out << "  <td>"                                                   << endl;
  *out << "</table>"                                        << std::endl;
  *out << "</body>"                                         << std::endl;
  *out << "</html>"                                         << std::endl;

}


void FilterUnitFramework::debugWebPage(xgi::Input *in, xgi::Output *out) 
  throw (xgi::exception::Exception)
{
  std::string url = "/";
  url += getApplicationDescriptor()->getURN();
  url += "/debug";
  cgicc::Cgicc cgi(in);
  if(runActive_)
    {
      vector<cgicc::FormEntry> el1;
      cgi.getElement("FedFragment",el1);
      if(el1.size()!=0)
	{
	  string fn;
	  fn = el1[0].getValue();
	  fed_id = atoi(fn.data());
	}
    }
  *out << "<html>"                                                   << endl;
  *out << "<head>"                                                   << endl;
  *out << "<link type=\"text/css\" rel=\"stylesheet\"";
  *out << " href=\"/" <<  getApplicationDescriptor()->getURN()
       << "/styles.css\"/>"                                          << endl;
  *out << "<title>" << getApplicationDescriptor()->getClassName() 
       << getApplicationDescriptor()->getInstance() 
       << " MAIN</title>"                                            << endl;
  *out << "</head>"                                                  << endl;
  *out << "<body>"                                                   << endl;
  *out << "<table border=\"0\" width=\"100%\">"                      << endl;
  *out << "<tr>"                                                     << endl;
  *out << "  <td align=\"left\">"                                    << endl;
  *out << "    <img"                                                 << endl;
  *out << "     align=\"middle\""                                    << endl;
  *out << "     src=\"/daq/evb/bu/images/debug64x64.gif\""     << endl;
  *out << "     alt=\"main\""                                        << endl;
  *out << "     width=\"64\""                                        << endl;
  *out << "     height=\"64\""                                       << endl;
  *out << "     border=\"\"/>"                                       << endl;
  *out << "    <b>"                                                  << endl;
  *out << getApplicationDescriptor()->getClassName() 
       << getApplicationDescriptor()->getInstance()                  << endl;
  *out << "      " << fsm_->stateName_.toString()                    << endl;
  *out << "    </b>"                                                 << endl;
  *out << "  </td>"                                                  << endl;
  *out << "  <td width=\"32\">"                                      << endl;
  *out << "    <a href=\"/urn:xdaq-application:lid=3\">"             << endl;
  *out << "      <img"                                               << endl;
  *out << "       align=\"middle\""                                  << endl;
  *out << "       src=\"/daq/xdaq/hyperdaq/images/HyperDAQ.jpg\""    << endl;
  *out << "       alt=\"HyperDAQ\""                                  << endl;
  *out << "       width=\"32\""                                      << endl;
  *out << "       height=\"32\""                                      << endl;
  *out << "       border=\"\"/>"                                     << endl;
  *out << "    </a>"                                                 << endl;
  *out << "  </td>"                                                  << endl;
  *out << "  <td width=\"32\">"                                      << endl;
  *out << "  </td>"                                                  << endl;
  *out << "  <td width=\"32\">"                                      << endl;
  *out << "    <a href=\"/" << getApplicationDescriptor()->getURN() 
       << "/Default\">"                   << endl;
  *out << "      <img"                                               << endl;
  *out << "       align=\"middle\""                                  << endl;
  *out << "       src=\"/daq/evb/examples/fu/images/fu32x32.gif\""   << endl;
  *out << "       alt=\"Default\""                                   << endl;
  *out << "       width=\"32\""                                      << endl;
  *out << "       height=\"32\""                                     << endl;
  *out << "       border=\"\"/>"                                     << endl;
  *out << "    </a>"                                                 << endl;
  *out << "  </td>"                                                  << endl;
  *out << "</tr>"                                                    << endl;
  *out << "</table>"                                                 << endl;

  *out << "<hr/>"                                                    << endl;
  *out << "<table>"                                                  << endl;
  *out << "<tr valign=\"top\">"                                      << endl;
  *out << "  <td>"                                                   << endl;
  *out << "  <td>"                                                   << endl;
  if(runActive_)
    {
      ostringstream os;
      os << fed_id;
      *out << cgicc::form().set("method","get").set("action", url).set("enctype","multipart/form-data") << std::endl;
      *out << cgicc::input().set("type","text").set("name","FedFragment").set("value",os.str()).set("active","true") << std::endl;
      *out << cgicc::input().set("type","submit").set("value","FedDump") << std::endl;         
      *out << cgicc::form() << std::endl;
    }
  *out << "  </td>"                                                   << endl;
  if(fed_id >= 0)
    {
      unsigned int fid = unsigned(fed_id);
      unsigned int size = factory_->spyBuiltEvent(fid,dump_);
      string sdump = evf::dumpFrame(dump_, size);
      *out << "<textarea rows=10 cols=80 scroll=yes>"          << endl;
      *out << sdump;
    }
  *out << "</textarea><P>"                                           << endl;
  *out << "  </td>"                                                   << endl;
  *out << "</table>"                                        << std::endl;
  
  *out << "<table frame=\"void\" rules=\"groups\" class=\"states\">" << std::endl;
  *out << "<colgroup> <colgroup align=\"rigth\">"                    << std::endl;
  //title
  {
    *out << "  <tr>"                                                   << endl;
    *out << "    <th colspan=2>"                                       << endl;
    *out << "      " << "Message Stats"                                << endl;
    *out << "    </th>"                                                << endl;
    *out << "  </tr>"                                                  << endl;
  }
  //headers
  {
    *out << "<tr>" << std::endl;
    *out << "<th >" << std::endl;
    *out << "Message" << std::endl;
    *out << "</th>" << std::endl;
    *out << "<th>" << std::endl;
    *out << "Sent/Received" << std::endl;
    *out << "</th>" << std::endl;
    *out << "</tr>" << std::endl;
  }
  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "Allocate(*)" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << nAllocateSent_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;
  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "Discard" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << nDiscardSent_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;
  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "Take" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << nTakeReceived_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;
  *out << "<tr background-color=\"green\">" << std::endl;
  *out << "<td>" << std::endl;
  *out << "(*)EventsAllocated" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << nAllocatedEvents_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;
  *out << "</table>" << std::endl;

  *out << "</body>"                                         << std::endl;
  *out << "</html>"                                         << std::endl;
}

void FilterUnitFramework::parameterTables(xgi::Input *in, xgi::Output *out)
{
  //configuration parameter table

  *out << "<table frame=\"void\" rules=\"groups\" class=\"states\">" << std::endl;
  *out << "<colgroup> <colgroup align=\"rigth\">"                    << std::endl;
  //title
  {
    *out << "  <tr>"                                                   << endl;
    *out << "    <th colspan=2>"                                       << endl;
    *out << "      " << "Configuration"                                << endl;
    *out << "    </th>"                                                << endl;
    *out << "  </tr>"                                                  << endl;
  }
  //headers
  {
    *out << "<tr>" << std::endl;
    *out << "<th >" << std::endl;
    *out << "Parameter" << std::endl;
    *out << "</th>" << std::endl;
    *out << "<th>" << std::endl;
    *out << "Value" << std::endl;
    *out << "</th>" << std::endl;
    *out << "</tr>" << std::endl;
  }
  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "buInstance" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << buInstance_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "runNumber" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << runNumber_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "doDump" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << doDumpFragments_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "doDrop" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << doDropFragments_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "doCrc" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << doCrcCheck_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "queueSize" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << queueSize_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "</table>" << std::endl;


  *out << "<table frame=\"void\" rules=\"rows\" class=\"modules\">" << std::endl;
  //title
  {
    *out << "  <tr>"                                                   << endl;
    *out << "    <th colspan=2>"                                       << endl;
    *out << "      " << "Application State"                            << endl;
    *out << "    </th>"                                                << endl;
    *out << "  </tr>"                                                  << endl;
  }
  //headers
  {
    *out << "<tr >" << std::endl;
    *out << "<th >" << std::endl;
    *out << "Parameter" << std::endl;
    *out << "</th>" << std::endl;
    *out << "<th >" << std::endl;
    *out << "Value" << std::endl;
    *out << "</th>" << std::endl;
    *out << "</tr>" << std::endl;
  }
  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "EventRate" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << MonitorEventsPerSec_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "EventsClaimed" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << nbEvents_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "EventsProcessed" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << nbProcessedEvents_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "EventsReceived" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << nbReceivedEvents_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "FragmentsReceived" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << nbReceivedFragments_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;

  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "PendingRequests" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << pendingRequests_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;
  
  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "DataErrors" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << nbDataErrors_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;
  
  *out << "<tr>" << std::endl;
  *out << "<td>" << std::endl;
  *out << "CrcErrors" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td" << ( doCrcCheck_.value_ !=0 ?  (nbCrcErrors_==0 ? ">" : " bgcolor=\"red\">" ) : 
   " bgcolor=\"grey\">" ) << std::endl;
  *out << nbCrcErrors_ << std::endl;
  *out << "</td>" << std::endl;
  *out << "</tr>" << std::endl;
  

  *out << "</table>"                                        << endl;

}

FURawEvent * FilterUnitFramework::rqstEvent()
{
  nbProcessedEvents_ = factory_->getnbProcessed();
  mutex_->take(); //this is used to atomically suspend the FU on next event
  //  cout << "FF::rqstEvent, queue size=" << factory_->queueSize() << "pending requests "
  //       << pendingRequests_ << endl;
  FURawEvent *ev = factory_->getBuiltEvent();
  mutex_->give();
  nbEvents_.value_++; // increment counter of claimed events 
  int request = queueSize_/2;
  if(queueSize_.value_ < 2)
    request = 1;
  if((factory_->queueSize() + pendingRequests_<=queueSize_/2) && !flush_)
    {
        unsigned int allocfset = 1;
	if(buMaxInstance_.value_ == 0)
	  sendAllocate(buInstance_, request, allocfset);
	else
	  {
	    if(bui_ < buMaxInstance_.value_) 
	      bui_++;
	    else
	      bui_ = buInstance_;
	    sendAllocate(bui_, request, allocfset);
	  }
    }

  return ev;
}

void FilterUnitFramework::signalWaitingInput()
{
  factory_->postEndRunMaybe();
}


/* Monitoring Timer Callback Function*/
void FilterUnitFramework::timeExpired (toolbox::task::TimerEvent& e)
{
  unsigned int nbproc = factory_->getnbProcessed();
  static xdata::UnsignedInteger32 nbLast_ = nbproc;
  xdata::UnsignedInteger32 nbDiff_;


  mutex_->take();
  s_mon->lock();

  nbDiff_=nbproc-nbLast_;
  nbLast_=nbproc;
  MonitorEventsPerSec_=((xdata::Double)nbDiff_)/(MonitorIntervalSec_);
  
  s_mon->unlock();
  mutex_->give();
}


void FilterUnitFramework::flushBuiltQueue()
{
  // flushed events are considered as processed by the eventfactory but the local counter is not incremented
  // one should not forget to reset the factory counter after a halt or else the count will keep incrementing the old value 
  while(factory_->queueSize() != 0)
    {
      FURawEvent *ev = factory_->getBuiltEvent();
      ev->reset(true);
      nbEvents_.value_++; // increment counter of claimed events 
    }
}

void FilterUnitFramework::actionPerformed (xdata::Event& e)
{
  if (e.type() == "ItemChangedEvent" && !(fsm_->stateName_.toString()=="Halted"))
    {
      std::string item = dynamic_cast<xdata::ItemChangedEvent&>(e).itemName();
      if ( item == "runNumber")
	resetCounters();
    }
}


void FilterUnitFramework::resetCounters()
{
  nbReceivedEvents_    = 0;
  nbProcessedEvents_   = 0;
  nbReceivedFragments_ = 0;
  //  pendingRequests_     = 0; // this should not be reset since the BU remembers across runs
  nbDataErrors_        = 0;
  nbCrcErrors_         = 0;
  MonitorEventsPerSec_ = 0;
  factory_->resetNbProcessed();
}

bool FilterUnitFramework::checkParameters(string &reason)
{
  ostringstream oreason;
  bool retVal = true;
  if(queueSize_.value_==0)
    {
      retVal = false;
      oreason << "Cannot set queueSize to " << queueSize_ << ". Invalid parameter value.";
    }
  else if(queueSize_.value_ == 1)
    {
      LOG4CPLUS_WARN(this->getApplicationLogger(),
		     "queueSize is set to 1. No events will be cached ");
    }
  else if(queueSize_.value_ > FURawEventFactory::maxHandles)
    {
      retVal = false;
      oreason << "Cannot set queueSize to " << queueSize_ << ". Not enough event handles, max "
	      <<  FURawEventFactory::maxHandles;
    }
  reason = oreason.str();
  return retVal;
}

XDAQ_INSTANTIATOR_IMPL(FilterUnitFramework)

