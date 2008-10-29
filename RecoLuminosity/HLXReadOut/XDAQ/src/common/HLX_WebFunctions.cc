/*************************************************************************
 * XDAQ layer for HLX card                                               *
 * Copyright (C) 2007, Princeton University                              *
 * All rights reserved.                                                  *
 * Author(s): J. Jones              				         *
 ************************************************************************/

#include "HLXReadoutSupervisor.hh"
#include <log4cplus/consoleappender.h>

 #include <sys/types.h>
#include <dirent.h>
#include <errno.h>
//#include <iomanip>
//#include <iostream>

using namespace log4cplus;
using namespace log4cplus::spi;
using namespace log4cplus::helpers;
using namespace std;
using namespace HCAL_HLX;

#define XDAQ_PAGE_HEADER "HCAL HLX Supervisor"

//namespace HCAL_HLX {

  void lumiHCALLUMIDAQ::WebShowTextOption(xgi::Output *out,
					std::string *title,
					std::string *setting,
					std::string *defaultValue,
					i32 boardID) throw (xgi::exception::Exception) {
    
    try {

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/ParameterSet";

      *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
      *out << "<table border=0 cellpadding=5 cellspacing=0 width=675 style=\"font-family:arial;font-size:10pt;color:#0000AA\"><tr>" << std::endl;
      *out << "<td style=\"font-weight:bold\" width=175>" << *title << ":</td><td>" << endl;
      *out << cgicc::input().set("type","hidden").set("name","setting").set("value",*setting) << std::endl;
      *out << cgicc::input().set("type","text").set("name","value").set("value",*defaultValue) << std::endl;
      if ( boardID >= 0 ) {
	std::stringstream tempStream;
	std::string tempString;
	tempStream << dec << boardID << endl; tempStream >> tempString; 
	*out << cgicc::input().set("type","hidden").set("name","board").set("value",tempString) << std::endl;	
      }
      *out << cgicc::input().set("type","submit").set("value","Update")  << std::endl;
      *out << cgicc::form() << "</td></tr></table>" << std::endl;

    } catch(xgi::exception::Exception &e) {
      XCEPT_RETHROW(xgi::exception::Exception, "Exception caught in WebShowSwitch", e);
    }
    
 }

  void lumiHCALLUMIDAQ::WebShowHexOption(xgi::Output *out,
				       std::string *title,
				       std::string *setting,
				       std::string *defaultValue,
				       HCAL_HLX::i32 boardID) throw (xgi::exception::Exception) {
    
    try {

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/ParameterSet";

      *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
      *out << "<table border=0 cellpadding=5 cellspacing=0 width=675 style=\"font-family:arial;font-size:10pt;color:#0000AA\"><tr>" << std::endl;
      *out << "<td style=\"font-weight:bold\" width=160>" << *title << ":</td><td>" << endl;
      *out << "<td style=\"font-family:arial;color:#000000\" width=15>0x</td><td>" << endl;
      *out << cgicc::input().set("type","hidden").set("name","setting").set("value",*setting) << std::endl;
      if ( boardID >= 0 ) {
	std::stringstream tempStream;
	std::string tempString;
	tempStream << dec << boardID << endl; tempStream >> tempString; 
	*out << cgicc::input().set("type","hidden").set("name","board").set("value",tempString) << std::endl;	
      }
      *out << cgicc::input().set("type","text").set("name","value").set("value",*defaultValue) << std::endl;
      *out << cgicc::input().set("type","submit").set("value","Update")  << std::endl;
      *out << cgicc::form() << "</td></tr></table>" << std::endl;

    } catch(xgi::exception::Exception &e) {
      XCEPT_RETHROW(xgi::exception::Exception, "Exception caught in WebShowSwitch", e);
    }
    
 }


  void lumiHCALLUMIDAQ::WebShowFourChannelHexOption(xgi::Output *out,
							 std::string *title,
							 std::string *setting,
							 std::string *defaultValue,
							 HCAL_HLX::i32 boardID) throw (xgi::exception::Exception) {
    
    try {
      string tempstring;
      std::stringstream temp;

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/ParameterSet";
 
      *out << "<table border=0 cellpadding=5 cellspacing=0 width=675 style=\"font-family:arial;font-size:10pt;color:#0000AA\">" << std::endl;
      for (int i=0; i<4; i++){

 //old       temp << *setting << (i+1) << std::endl;
        temp << *setting <<"."<< (i+1) << std::endl;   //new using numoption
        temp >> tempstring;

 	*out << "<tr><td style=\"font-weight:bold\" width=160>Ch." << (i+1) << " " << *title << ":</td>" << endl;
	*out << "<td style=\"font-family:arial;color:#000000\" width=15>0x</td><td>" << endl;

        *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;

	*out << cgicc::input().set("type","hidden").set("name","setting").set("value",tempstring) << std::endl;

      if ( boardID >= 0 ) {
	std::stringstream tempStream;
	std::string tempString;
	tempStream << dec << boardID << endl; tempStream >> tempString; 
	*out << cgicc::input().set("type","hidden").set("name","board").set("value",tempString) << std::endl;	
      }

	*out << cgicc::input().set("type","text").set("name","value").set("value",defaultValue[i]) << std::endl;
	*out << cgicc::input().set("type","submit").set("value","Update")  << std::endl;
	*out << cgicc::form() << "</td></tr>" << std::endl;
      }

      *out << "</table>" << std::endl;


    } catch(xgi::exception::Exception &e) {
      XCEPT_RETHROW(xgi::exception::Exception, "Exception caught in WebShowSwitch", e);
    }
    
 }





  void lumiHCALLUMIDAQ::WebShowHighlightOption(xgi::Output *out,
						    std::string *title,
						    std::string *setting,
						    HCAL_HLX::u32 numOptions,
						    HCAL_HLX::u32 currentOption,
						    std::string *optionValues,
						    std::string *optionDisplays,
						    HCAL_HLX::i32 boardID) throw (xgi::exception::Exception) {
    
    try {

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/ParameterSet";

      *out << "<table border=0 cellpadding=5 cellspacing=0 width=675 style=\"font-family:arial;font-size:10pt;color:#0000AA\"><tr>" << endl;
      *out << "<td style=\"font-weight:bold\" width=175>" << *title << ":</td>" << endl;
      for ( u32 i = 0 ; i != numOptions ; i++ ) {
	if ( currentOption != i ) {
	  *out << "<td width=90 align=\"center\"><a style=\"font-size:10pt\" href=\"" << cmdLink << "?setting=" << *setting;
	  if ( boardID >= 0 ) {
	    *out << "&board=" << boardID << std::endl;	
	  }
	  *out << "&value=" << optionValues[i] << "\">" << optionDisplays[i] << "</a></td>" << endl;
	} else {
	  *out << "<td style=\"font-weight:bold;color:#008800;font-size:10pt\" width=90 bgcolor=\"#DDFFDD\" align=\"center\">" << optionDisplays[i] << "</td>" << endl;
	}
      }
     if (numOptions<5) *out << "<td>&nbsp;</td>";
     *out << "</tr></table>";
      
    } catch(xgi::exception::Exception &e) {
      XCEPT_RETHROW(xgi::exception::Exception, "Exception caught in WebShowSwitch", e);
    }
    
 }


  void lumiHCALLUMIDAQ::WebShowHighlightOption(xgi::Output *out,
						    std::string *title,
						    std::string *setting,
						    HCAL_HLX::u32 numOptions,
						    const bool *currentOption,
						    std::string *optionValues,
						    std::string *optionDisplays,
						    HCAL_HLX::i32 boardID) throw (xgi::exception::Exception) {
    
    try {

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/ParameterSet";

      *out << "<table border=0 cellpadding=5 cellspacing=0 width=675 style=\"font-family:arial;font-size:10pt;color:#0000AA\"><tr>" << endl;
      *out << "<td style=\"font-weight:bold\" width=175>" << *title << ":</td>" << endl;
      for ( u32 i = 0 ; i != numOptions ; i++ ) {

	if ( currentOption[i] != true ) {
	  *out << "<td width=90 align=\"center\"><a style=\"font-size:10pt\" href=\"" << cmdLink << "?setting=" << *setting;
	  if ( boardID >= 0 ) {
	    *out << "&board=" << boardID << endl;	
	  }
	  *out << "&value=" << optionValues[i] << "\">" << optionDisplays[i] << "</a></td>" << endl;
	} else {
	  *out << "<td width=90 bgcolor=\"#DDFFDD\" align=\"center\"><a style=\"color:#008800;font-weight:bold;font-size:10pt;text-decoration:none\" href=\"" << cmdLink << "?setting=" << *setting << "&value=" << optionValues[i] << "\">" << optionDisplays[i] << "</font></a></td>" << endl;
	} 
      }
     if (numOptions<5) *out << "<td>&nbsp;</td>";
     *out << "</tr></table>";
      
    } catch(xgi::exception::Exception &e) {
      XCEPT_RETHROW(xgi::exception::Exception, "Exception caught in WebShowSwitch", e);
    }
    
 }

  /*
  void lumiHCALLUMIDAQ::TransitionFSMHalt(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception) {
    try {
      *out << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
      *out << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
      *out << cgicc::title(XDAQ_PAGE_HEADER) << std::endl;
      xgi::Utils::getPageHeader(*out, XDAQ_PAGE_HEADER);      
      std::string link = "/";
      link += getApplicationDescriptor()->getURN().c_str();
      link += "/";
      link += mLastPage;
      *out << "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"0; URL=" << link << "\">" ;
      this->TriggerFSMTransition("Halt");
    } catch(ICException &aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xgi::exception::Exception, errMsg);
    }
  }

  void lumiHCALLUMIDAQ::TransitionFSMEnable(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception) {
    try {
      *out << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
      *out << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
      *out << cgicc::title(XDAQ_PAGE_HEADER) << std::endl;
      xgi::Utils::getPageHeader(*out, XDAQ_PAGE_HEADER);      
      std::string link = "/";
      link += getApplicationDescriptor()->getURN().c_str();
      link += "/";
      link += mLastPage;
      *out << "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"0; URL=" << link << "\">" ;
      this->TriggerFSMTransition("Enable");
    } catch(ICException &aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xgi::exception::Exception, errMsg);
    }
  }

  void lumiHCALLUMIDAQ::TransitionFSMConfigure(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception) {
    try {
      *out << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
      *out << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
      *out << cgicc::title(XDAQ_PAGE_HEADER) << std::endl;
      xgi::Utils::getPageHeader(*out, XDAQ_PAGE_HEADER);      
      std::string link = "/";
      link += getApplicationDescriptor()->getURN().c_str();
      link += "/";
      link += mLastPage;
      *out << "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"0; URL=" << link << "\">" ;
      this->TriggerFSMTransition("Configure");
    } catch(ICException &aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xgi::exception::Exception, errMsg);
    }
  }
  */

  void lumiHCALLUMIDAQ::WebShowFSMControls(xgi::Output * out) throw (xgi::exception::Exception) {
    try {
      //mHardwareManager->QueryConfigStatusMatch();
      //std::string state = fsm_.getStateName(fsm_.getCurrentState());
      /*if ( !mHardwareManager->QueryConfigStatusMatch() ) {
	*out << "<p style=\"font-weight:bold;color:#cc0000\">Board configuration/status mismatch - must execute configure!</p>" << endl;
	if ( state != "Init" ) {
	  // Jump back to the init state to force a configure transition
	  this->TriggerFSMTransition("Init");
	}
	} else {*/
      /*	*out << "<p style=\"font-weight:bold;color:#00cc00\">Boards' configurations are valid</p>" << endl;	
	//}
      *out << "<p style=\"font-family:arial;font-size:12pt;color:#000055\">Current hardware state: ";
      state = fsm_.getStateName(fsm_.getCurrentState());
      if ( state == "Init" ) {
	std::string configlink = toolbox::toString("/%s/TransitionFSMConfigure",getApplicationDescriptor()->getURN().c_str());
        *out << "INIT [<a style=\"color:#0099CC\" href="
	     << configlink << ">CONFIGURE</a>]</p>" << endl;
      } else if ( state == "Configured" ) {
	std::string enablelink = toolbox::toString("/%s/TransitionFSMEnable",getApplicationDescriptor()->getURN().c_str());
        *out << "CONFIGURED [<a style=\"color:#0099CC\" href="
	     << enablelink << ">ENABLE</a>]</p>" << endl;
      } else if ( state == "Enabled" ) {
	std::string haltlink = toolbox::toString("/%s/TransitionFSMHalt",getApplicationDescriptor()->getURN().c_str());
        *out << "ENABLED [<a style=\"color:#0099CC\" href="
	     << haltlink << ">HALT</a>]</p>" << endl;
      } else if ( state == "Halted" ) {
	std::string enablelink = toolbox::toString("/%s/TransitionFSMEnable",getApplicationDescriptor()->getURN().c_str());
        *out << "HALTED [<a style=\"color:#0099CC\" href="
	     << enablelink << ">ENABLE</a>]</p>" << endl;
	     }*/
    } catch (ICException & aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xgi::exception::Exception, errMsg);
    }
  }

  void lumiHCALLUMIDAQ::WebConstructPageHeader(xgi::Output * out, const std::string &title) throw (xgi::exception::Exception) {
    try {
      //cgicc::Cgicc cgi(in);
      *out << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
      *out << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
      *out << cgicc::title(XDAQ_PAGE_HEADER) << std::endl;
      xgi::Utils::getPageHeader(*out, XDAQ_PAGE_HEADER);
      *out << "<div style=\"font-size:24pt;font-weight:bold\">" << title << "</div><br/>" << endl; 
      this->WebShowGenericLinks(out);
      //*out << "<table width=\"100%\" height=\"4\" style=\"font-size:6pt;background:#ddeeff\" cellpadding=0 margin=0><tr width=\"*\"><td>&nbsp;</td></tr></table>" << endl;
      //this->WebShowFSMControls(out);
      *out << "<table width=\"100%\" height=\"4\" style=\"font-size:6pt;background:#ddeeff\" cellpadding=0 margin=0><tr width=\"*\"><td>&nbsp;</td></tr></table><br/>" << endl;
    } catch (ICException & aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xgi::exception::Exception, errMsg);
    }
  }

void lumiHCALLUMIDAQ::WebShowGenericLinks(xgi::Output * out) throw (xgi::exception::Exception) {
  
  std::string globalstatlink = toolbox::toString("/%s/Default",getApplicationDescriptor()->getURN().c_str());
  std::string configlink = toolbox::toString("/%s/WebConfig",getApplicationDescriptor()->getURN().c_str());
  
  *out << "<div style=\"font-family:arial;font-size:10pt;color:#0000AA\">[<a href="
       << globalstatlink << ">Status</a>] [<a href="
       << configlink << ">Configuration</a>]" // [<a href="
       << "</div><br/>";
  
}

