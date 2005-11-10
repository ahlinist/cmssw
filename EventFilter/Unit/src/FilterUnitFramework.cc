//
/// See header file for a description of this class
//
//
/// Author: E.Meschi PH/CMD
/// 
//
//  Modification history:
//    $Log: FilterUnitFramework.cc,v $
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

#include "toolbox/include/toolbox/Chrono.h"
#include "xcept/include/xcept/tools.h"
#include "xoap/include/xoap/MessageFactory.h"
#include "xoap/include/xoap/Method.h"
#include "xgi/include/xgi/Method.h"

#include "EventFilter/Unit/interface/FilterUnitFramework.h"
#include "EventFilter/Unit/interface/FURawEventFactory.h"

#include <pthread.h>
#include <sys/types.h>
#include <time.h>

FilterUnitFramework::FilterUnitFramework(xdaq::ApplicationStub *s) : FUAdapter(s),
								     nbEvents_(0),
								     flush_(false),
								     fsm_(0)
{
  cout << "Entered constructor " << endl;

  mutex_ = new BSem(BSem::FULL);

  fsm_ = new evf::EPStateMachine(getApplicationLogger());
  fsm_->init<FilterUnitFramework>(this);

  xoap::bind(this, &FilterUnitFramework::getStateMsg,"getState",XDAQ_NS_URI);

  exportParams();
  
  // Bind web interface
   xgi::bind(this, &FilterUnitFramework::css           , "styles.css");
   xgi::bind(this, &FilterUnitFramework::defaultWebPage, "Default");
  
  LOG4CPLUS_INFO(this->getApplicationLogger(),
		 xmlClass_ << instance_ << " constructor");
  
  cout << xmlClass_ << instance_ << " constructor" << endl;
  pthread_mutex_init(&lock_,0);
  pthread_cond_init(&ready_,0);


}


void FilterUnitFramework::exportParams()
{
  xdata::InfoSpace *s = getApplicationInfoSpace();
  // default configuration
  buInstance_     = 0;
  queueSize_ = 16;
  add_ = "localhost";
  port_ = 9090;
  del_ = 5000000;
  ostringstream ns;
  instance_ = getApplicationDescriptor()->getInstance();
  ns << "FU" << instance_;
  nam_ = ns.str();
  runActive_ = false;
  runNumber_ = 1;
  s->fireItemAvailable("buInstance",&buInstance_);

  // additional configuration specific to parametric FU

  s->fireItemAvailable("queueSize",&queueSize_);
  s->fireItemAvailable("stateName",&fsm_->stateName_);
  s->fireItemAvailable("collectorAddr",&add_);
  s->fireItemAvailable("collectorPort",&port_);
  s->fireItemAvailable("collSendUs",&del_);
  s->fireItemAvailable("monSourceName",&nam_);
  s->fireItemAvailable("runActive",&runActive_);
  s->fireItemAvailable("runNumber",&runNumber_);
}

FilterUnitFramework::~FilterUnitFramework()
{
  
  delete factory_;

}

#include <typeinfo>

#include <unistd.h>
#include <sys/utsname.h>

void FilterUnitFramework::configureAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  if(!runActive_) findOrCreateMemoryPool();

  char *workdir = getenv("PWD");
  if(workdir != 0)
    chdir(workdir);
  LOG4CPLUS_INFO(this->getApplicationLogger(),"FU Working Directory set to" << workdir);

  createBUArray();
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
  sendAllocate(buInstance_, queueSize_, allocfset);

  nbEvents_ = 0;       	  
  LOG4CPLUS_INFO(this->getApplicationLogger(),"Run Started...");
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

#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
void FilterUnitFramework::haltAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{

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


#include "i2o/utils/include/i2o/utils/AddressMap.h"
#include "extern/cgicc/linuxx86/include/cgicc/CgiDefs.h"
#include "extern/cgicc/linuxx86/include/cgicc/Cgicc.h"
#include "extern/cgicc/linuxx86/include/cgicc//FormEntry.h"
#include "extern/cgicc/linuxx86/include/cgicc/HTTPHTMLHeader.h"
#include "extern/cgicc/linuxx86/include/cgicc/HTMLClasses.h"
#include <stdio.h>

using namespace cgicc;

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
  *out << "EventsProcessed" << std::endl;
  *out << "</td>" << std::endl;
  *out << "<td>" << std::endl;
  *out << nbEvents_ << std::endl;
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
  

  *out << "</table>"                                        << endl;

}

FURawEvent * FilterUnitFramework::rqstEvent()
{
  mutex_->take(); //this is used to atomically suspend the FU on next event
  //  cout << "FF::rqstEvent, queue size=" << factory_->queueSize() << "pending requests "
  //       << pendingRequests_ << endl;
  FURawEvent *ev = factory_->getBuiltEvent();
  mutex_->give();
  nbEvents_.value_++; // increment counter of processed events 

  if((factory_->queueSize() + pendingRequests_<=queueSize_/2) && !flush_)
    {
        unsigned int allocfset = 1;
	sendAllocate(buInstance_,queueSize_/2,allocfset);
    }

  return ev;
}

XDAQ_INSTANTIATOR_IMPL(FilterUnitFramework);

