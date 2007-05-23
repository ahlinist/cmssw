#include "FEDMonitorClient.h"

#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include <iostream>
#include <typeinfo>
#include <map>
#include <sstream>

#include "xgi/include/xgi/Method.h"
#include "xgi/include/xgi/Utils.h"

#include "extern/cgicc/linuxx86/include/cgicc/Cgicc.h"
#include "extern/cgicc/linuxx86/include/cgicc/FormEntry.h"
#include "extern/cgicc/linuxx86/include/cgicc/HTMLClasses.h"

using namespace std;
namespace dqm{
  namespace daq{


    FEDMonitorClient::FEDMonitorClient( const edm::ParameterSet& pset) : 
      ModuleWeb("FEDMonitorClient")
     , qtHandle_(0)
     , nUpdates_(0)
     , collationtodo_(pset.getUntrackedParameter<bool>("doCollation",false)) 
     , qtesttodo_(pset.getUntrackedParameter<bool>("doQTests",false))
     , autorefresh_(false)
    {
      
      // get hold of back-end interface
      dbe = edm::Service<DaqMonitorBEInterface>().operator->();
      
      // instantiate Monitor UI without connecting to any monitoring server
      // (i.e. "standalone mode")
      mui = new MonitorUIRoot();
      
      dbe->showDirStructure();

      if(collationtodo_) bookCollationMaybe();       
      if(qtesttodo_) bookQTests(pset); 

    }
    void FEDMonitorClient::analyze(const edm::Event & e, const edm::EventSetup& c)
    {
      eventId_ = e.id().event();
      runId_ = e.id().run();
      nUpdates_++;
    }
    
    void FEDMonitorClient::endLuminosityBlock(edm::LuminosityBlock const &lb, edm::EventSetup const &es)
    {
      std::cout << "here I will do something intelligent later " << std::endl;
    }
    void FEDMonitorClient::bookQTests(const edm::ParameterSet &pset)
    {
      qtHandle_ = new QTestHandle;
      qtHandle_->configureTests(pset.getUntrackedParameter<std::string>("qtList", "QualityTests.xml"),mui);
      if(qtHandle_) qtHandle_->attachTests(mui);      
    }
    
    void FEDMonitorClient::bookCollationMaybe()
    {
      for(int i=0; i<1024; i++)
	{
	  std::ostringstream oss1;
	  oss1 << "fed" << i; 
	  CollateMonitorElement *cme = mui->collate1D(oss1.str(),oss1.str(),"FEDs/Collated");
	  std::ostringstream oss2;
	  oss2 << "*/FEDs/Details/"<<oss1.str();
	  mui->add(cme,oss2.str());
	}
    }
    
    
    void FEDMonitorClient::defaultWebPage(xgi::Input *in, xgi::Output *out)
    {

      try 
	{
	  cgicc::Cgicc cgi(in);
	  if ( xgi::Utils::hasFormElement(cgi,"autorefresh_") )
	    autorefresh_ = xgi::Utils::getFormElement(cgi, "autorefresh_")->getIntegerValue()  != 0;
	}
      catch (const std::exception & e) 
    {
      // don't care if it did not work
    }


        using std::endl;
	*out << "<html>"                                                   << endl;
	*out << "<head>"                                                   << endl;
	
	*out << "<title>" << typeid(FEDMonitorClient).name()
	     << " MAIN</title>"                                            << endl;
	if (autorefresh_) {
	  *out << "<meta http-equiv=\"refresh\" content=\"3\">"            << endl;
	}
	
	*out << "</head>"                                                  << endl;
	*out << "<body>"                                                   << endl;

	*out << cgicc::form().set("method","GET").set("action", std::string("/moduleWeb\?module=FEDMonitorClient") ) 
	     << std::endl;
	*out << cgicc::input().set("type","hidden").set("name","module=FEDMonitorClient\?autorefresh_").set("value", autorefresh_?"0":"1") 
	     << std::endl;
	*out << cgicc::input().set("type","submit").set("value",autorefresh_?"Toggle AutoRefresh OFF":"Toggle AutoRefresh ON")  
	     << std::endl;
	*out << cgicc::form()						   << std::endl;  


	*out << "<p>Run: " << runId_ << " Total updates: " << nUpdates_ << " Last Event analyzed: "
	     << eventId_;


	*out << " FED Monitor status "		                           << endl;


	int status = mui->getSystemStatus();
	switch(status)
	  {
	  case dqm::qstatus::ERROR:
	    *out << " Error(s)";
	    break;
	  case dqm::qstatus::WARNING:
	    *out << " Warning(s)";
	    break;
	  case dqm::qstatus::OTHER:
	    *out << " Some tests did not run;";
	    break; 
	  default:
	    *out << (nUpdates_ > 2 ? " No problems" : "Starting");
	  }

	*out << "<table>"						<< endl;
	*out << "<tr><th>SubSystem</th><th>Present/Nominal</th><th>Errors</th><th>Warnings</th><th>Other</th></tr>" << endl;
	
	/*
	std::vector<std::string> contents; 
	mui->getContents(contents);
	for(unsigned int i = 0; i < contents.size() ; i++)
	  *out << contents[i] << std::endl;
	  */
	//	std::vector<MonitorElement*> plots = dbe->getContents("FEDs/Collated");
	int sstn = 0;
	int sste = 0;
	int sstw = 0;
	int ssto = 0;
	pair<int, int> range = FEDNumbering::getSiStripFEDIds();
	int sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">SiStrip </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getSiPixelFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">SiPixel </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getDTFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">DT </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;


	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getCSCFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">CSC </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getDTTFFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">DTTF </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;


	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getRPCFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">RPC </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getPreShowerFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">PreShower </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getEcalFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">Ecal </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getHcalFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">Hcal </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getTriggerGTPFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
		*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">GTP </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getTriggerEGTPFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
		*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">EGTP </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;


	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getCSCTFFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
		*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">CSCTF </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	range = FEDNumbering::getDAQeFEDFEDIds();
	sstt = range.second - range.first + 1;
	for (int i = range.first; i <= range.second; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = mui->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
		*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">eFED </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	*out << "</table>"                                                 << endl;
	*out << "</body>"                                                  << endl;
	*out << "</html>"                                                  << endl;
    }
    std::string FEDMonitorClient::findSubsystemPath(std::string &subs)
    {
      return subs;
    }

  }
}
