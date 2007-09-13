#include "DQM/CSCMonitorClient/interface/CSCMonitorWebClient.h"


CSCMonitorWebClient::CSCMonitorWebClient(xdaq::ApplicationStub *stub)
  : DQMBaseClient(
		  stub,       // the application stub - do not change
		  "CSCMonitorClient",     // the name by which the collector identifies the client
		  "localhost",// the name of the computer hosting the collector
		  9090,       // the port at which the collector listens
		  5,          // the delay between reconnect attempts
		  false       // do not act as server
		  )
{
  // Instantiate a web interface:
  webInterface_p = new CSCMonitorWebClientInterface(getContextURL(),getApplicationURL(), & mui_);
  
  xgi::bind(this, &CSCMonitorWebClient::handleWebRequest, "Request");
}

void CSCMonitorWebClient::Default(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception)
{
  *out << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
  *out << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
  std::string url = getApplicationDescriptor()->getContextDescriptor()->getURL() + "/" + getApplicationDescriptor()->getURN();
  *out << "<frameset rows=\"200,90%\">" << std::endl;
  *out << "  <frame src=\"" << url << "/fsm" << "\">" << std::endl;
  *out << "  <frame src=\"" << url << "/general" << "\">" << std::endl;
  *out << "</frameset>" << std::endl;
  *out << cgicc::html() << std::endl;
}


/*
  implement the method that outputs the page with the widgets (declared in DQMBaseClient):
*/
void CSCMonitorWebClient::general(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception)
{
  // the web interface should know what to do:
  webInterface_p->Default(in, out);
}


/*
  the method called on all HTTP requests of the form ".../Request?RequestID=..."
*/
void CSCMonitorWebClient::handleWebRequest(xgi::Input * in, xgi::Output * out)
{
  // the web interface should know what to do:
  webInterface_p->handleRequest(in, out);
}

/*
  this obligatory method is called whenever the client enters the "Configured" state:
*/
void CSCMonitorWebClient::configure()
{

}

/*
  this obligatory method is called whenever the client enters the "Enabled" state:
*/
void CSCMonitorWebClient::newRun()
{
  upd_->registerObserver(this);
}

/*
  this obligatory method is called whenever the client enters the "Halted" state:
*/
void CSCMonitorWebClient::endRun()
{
}

/*
  this obligatory method is called by the Updater component, whenever there is an update 
*/
void CSCMonitorWebClient::onUpdate() const
{
  // put here the code that needs to be executed on every update:
  std::vector<std::string> uplist;
  // mui_->getUpdatedContents(uplist);
}
