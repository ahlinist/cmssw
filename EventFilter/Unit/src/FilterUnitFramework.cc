//
/// See header file for a description of this class
//
//
/// Author: E.Meschi PH/CMD
/// 
//
//  Modification history:
//    $Log: FilterUnitFramework.cc,v $
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
								     stateName_("Unknown"),
								     flush_(false)
{
  cout << "Entered constructor " << endl;

  mutex_ = new BSem(BSem::FULL);

  bindFSMSoapCallbacks();
  xoap::bind(this, &FilterUnitFramework::getStateMsg,"getState",XDAQ_NS_URI);

  exportParams();
  try
    {
      defineFSM();
    }
  catch(xcept::Exception e)
    {
      LOG4CPLUS_FATAL(this->getApplicationLogger(),
		      "Failed to define FSM "
		      << xcept::stdformat_exception_history(e));
      return;
    }
  
  // Bind default web page
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
  nTask_     = 1;
  add_ = "localhost";
  port_ = 9090;
  del_ = 5000000;
  ostringstream ns;
  instance_ = getApplicationDescriptor()->getInstance();
  ns << "FU" << instance_;
  nam_ = ns.str();
  runActive_ = true;
  runNumber_ = 1;
  dataSet_ ="Unknown";
  s->fireItemAvailable("buInstance",&buInstance_);

  // additional configuration specific to parametric FU

  s->fireItemAvailable("queueSize",&queueSize_);
  s->fireItemAvailable("nTask",&nTask_);
  s->fireItemAvailable("stateName",&stateName_);
  s->fireItemAvailable("collectorAddr",&add_);
  s->fireItemAvailable("collectorPort",&port_);
  s->fireItemAvailable("collSendUs",&del_);
  s->fireItemAvailable("monSourceName",&nam_);
  s->fireItemAvailable("runActive",&runActive_);
  s->fireItemAvailable("runNumber",&runNumber_);
  s->fireItemAvailable("dataSet",&dataSet_);
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

  char *workdir = getenv("PWD");
  if(workdir != 0)
    chdir(workdir);
  LOG4CPLUS_INFO(this->getApplicationLogger(),"FU Working Directory set to" << workdir);

  createBUArray();

  stateName_ = fsm_.stateName_.toString();  
}

void FilterUnitFramework::enableAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  cout<<"FilterUnitFramework::Enable()"<<endl;

  birth_.start(0); // start timer before sending allocate

  //
  // Start operation by requesting the first queueSize_ events
  //
  // (Fill in the event queue)

  unsigned int allocfset = 1;
  LOG4CPLUS_INFO(this->getApplicationLogger(),"FU sends initial allocate N=" << queueSize_);
  sendAllocate(buInstance_, queueSize_, allocfset);

  nbEvents_ = 0;       	  
  
  mutex_->take();
  LOG4CPLUS_INFO(this->getApplicationLogger(),"Run Paused...");
  stateName_ = fsm_.stateName_.toString();
  mutex_->give();

  pthread_mutex_lock(&lock_);
  pthread_cond_wait(&ready_,&lock_);
  pthread_mutex_unlock(&lock_);

  LOG4CPLUS_INFO(this->getApplicationLogger(),"Run Started...");
}

void FilterUnitFramework::suspendAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  //  pthread_t ttid = pthread_self();

  mutex_->take();
  stateName_ = fsm_.stateName_.toString();
  LOG4CPLUS_INFO(this->getApplicationLogger(),"Run Paused...");
}

void FilterUnitFramework::resumeAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  //  pthread_t ttid = pthread_self();

  mutex_->give();
  stateName_ = fsm_.stateName_.toString();
  LOG4CPLUS_INFO(this->getApplicationLogger(),"Run Resumed...");
}  

#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
void FilterUnitFramework::haltAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception)
{
  // this will cause cobra to shut down
  //  system("touch CARF.stop");

  // should do more than this ? Maybe I need more states to comply with 
  // the definitions of Stop and Suspend... 
  stateName_ = fsm_.stateName_.toString();
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
  stateElement.addTextNode(fsm_.stateName_);
  
  return responseMsg;
}

xoap::MessageReference FilterUnitFramework::createFSMReplyMsg(const string cmd, const string state)
{
  stateName_ = fsm_.stateName_.toString();
  xoap::MessageReference msg = xoap::createMessage();
  xoap::SOAPEnvelope     env   = msg->getSOAPPart().getEnvelope();
  xoap::SOAPBody         body  = env.getBody();
  string                 rStr  = cmd + "Response";
  xoap::SOAPName         rName = env.createName(rStr,"xdaq",XDAQ_NS_URI);
  xoap::SOAPBodyElement  rElem = body.addBodyElement(rName);
  xoap::SOAPName       sName = env.createName("state","xdaq",XDAQ_NS_URI);
  xoap::SOAPElement      sElem = rElem.addChildElement(sName);
  xoap::SOAPName   aName = env.createName("stateName","xdaq",XDAQ_NS_URI);
  
  
  sElem.addAttribute(aName, state);
  
  return msg;
}

void FilterUnitFramework::bindFSMSoapCallbacks()
{
  xoap::bind(this,&FilterUnitFramework::fireEvent,"Configure", XDAQ_NS_URI);
  xoap::bind(this,&FilterUnitFramework::fireEvent,"Enable"   , XDAQ_NS_URI);
  xoap::bind(this,&FilterUnitFramework::fireEvent,"Suspend"  , XDAQ_NS_URI);
  xoap::bind(this,&FilterUnitFramework::fireEvent,"Resume"   , XDAQ_NS_URI);
  xoap::bind(this,&FilterUnitFramework::fireEvent,"Halt"     , XDAQ_NS_URI);
  xoap::bind(this,&FilterUnitFramework::fireEvent,"Disable"  , XDAQ_NS_URI); 
  xoap::bind(this,&FilterUnitFramework::fireEvent,"Fail"     , XDAQ_NS_URI); 
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
  
  cgicc::Cgicc cgi(in);
  string stateName   = fsm_.stateName_.toString();  
  bool configured = (stateName=="Ready" || stateName=="Enabled");
  std::string url = "/";
  url += getApplicationDescriptor()->getURN();

        string stateColour = (stateName == "Failed") ? "#FF4040" : "#AFECF7";
	int tid            = i2o::utils::getAddressMap()->getTid(getApplicationDescriptor());

	*out << HTMLDoctype(HTMLDoctype::eStrict)                 << std::endl;

        *out << "<html>"                                          << std::endl;

        *out << "<head>"                                          << std::endl;
        *out << "<title>" << xmlClass_ << instance_ << "</title>" << std::endl;
        *out << "</head>"                                         << std::endl;

        *out << "<body bgcolor=\"#007F7F\">"                      << std::endl;

        *out << "<table border=1 bgcolor=\"#CFCFCF\">"            << std::endl;

        *out << "<tr>"                                            << std::endl;
        *out << "  <td>"                                          << std::endl;
        *out << "    <b>"                                         << std::endl;
        *out << "      ";
        *out << xmlClass_ << instance_ << " tid:" << tid          << std::endl;
        *out << "    </b>"                                        << std::endl;
        *out << "  </td>"                                         << std::endl;
        *out << "  <td bgcolor=\"" << stateColour << "\">"        << std::endl;
        *out << "    " << stateName                               << std::endl;
        *out << "  </td>"                                         << std::endl;
	*out << "  <td bgcolor=\"" << "#aaaaaa" << "\">"          << std::endl;
	*out << " Run " << runNumber_ << "</td>"                  << std::endl;
	if(runActive_ && configured)
	  {
	    *out << "  <td bgcolor=\"" << stateColour << "\">"    << std::endl;
	    *out << "    " << "active"                            << std::endl;
	  }
	else if(configured)
	  {
	    *out << "  <td bgcolor=\"" << "#FF4040" << "\">"      << std::endl;
	    *out << "    " << "stopped"                           << std::endl;
	  }
        *out << "  </td>"                                         << std::endl;
	*out << "  <td bgcolor=\"" << "#aaaaaa" << "\">"          << std::endl;
	*out << " DataSet " << dataSet_.value_ << "</td>"          << std::endl;

        *out << "</tr>"                                           << std::endl;

        *out << "</table>"                                        << std::endl;
        *out << "<table border=1 bgcolor=\"#CFCFCF\">" << std::endl;

        *out << "<tr>" << std::endl;
        *out << "<td bgcolor=\"#F2F458\">" << std::endl;
        *out << "buInstance" << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#AFECF7\">" << std::endl;
        *out << buInstance_ << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#F2F458\">" << std::endl;
        *out << "doDump" << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#AFECF7\">" << std::endl;
        *out << doDumpFragments_ << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#F2F458\">" << std::endl;
        *out << "queueSize" << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#AFECF7\">" << std::endl;
        *out << queueSize_ << std::endl;
        *out << "</td>" << std::endl;
        *out << "</tr>" << std::endl;

        *out << "<tr>" << std::endl;
        *out << "<td bgcolor=\"#F2F458\">" << std::endl;
        *out << "EventsProcessed" << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#AFECF7\">" << std::endl;
        *out << nbEvents_ << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#F2F458\">" << std::endl;
        *out << "EventsReceived" << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#AFECF7\">" << std::endl;
        *out << nbReceivedEvents_ << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#F2F458\">" << std::endl;
        *out << "FragmentsReceived" << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#AFECF7\">" << std::endl;
        *out << nbReceivedFragments_ << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#F2F458\">" << std::endl;
        *out << "PendingRequests" << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#AFECF7\">" << std::endl;
        *out << pendingRequests_ << std::endl;
        *out << "</td>" << std::endl;
        *out << "</tr>" << std::endl;

        *out << "<tr>" << std::endl;
        *out << "<td bgcolor=\"#F2F458\">" << std::endl;
        *out << "DataErrors" << std::endl;
        *out << "</td>" << std::endl;
        *out << "<td bgcolor=\"#AFECF7\">" << std::endl;
        *out << nbDataErrors_ << std::endl;
        *out << "</td>" << std::endl;
        *out << "</tr>" << std::endl;

	*out << "</table>" << std::endl;
        *out << "</body>"                                         << std::endl;
        *out << "</html>"                                         << std::endl;

}

// leave out for now

FURawEvent * FilterUnitFramework::rqstEvent()
{
  pthread_mutex_lock(&lock_);
  pthread_cond_signal(&ready_);
  pthread_mutex_unlock(&lock_);
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

