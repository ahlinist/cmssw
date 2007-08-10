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

namespace HCAL_HLX {

  void HLXReadoutSupervisor::WebShowTextOption(xgi::Output *out,
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

  void HLXReadoutSupervisor::WebShowHexOption(xgi::Output *out,
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


  void HLXReadoutSupervisor::WebShowFourChannelHexOption(xgi::Output *out,
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





  void HLXReadoutSupervisor::WebShowHighlightOption(xgi::Output *out,
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


  void HLXReadoutSupervisor::WebShowHighlightOption(xgi::Output *out,
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

  void HLXReadoutSupervisor::TransitionFSMHalt(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception) {
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

  void HLXReadoutSupervisor::TransitionFSMEnable(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception) {
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

  void HLXReadoutSupervisor::TransitionFSMConfigure(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception) {
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

  void HLXReadoutSupervisor::WebShowFSMControls(xgi::Output * out) throw (xgi::exception::Exception) {
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

  void HLXReadoutSupervisor::WebConstructPageHeader(xgi::Output * out, const std::string &title) throw (xgi::exception::Exception) {
    try {
      //cgicc::Cgicc cgi(in);
      *out << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
      *out << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
      *out << cgicc::title(XDAQ_PAGE_HEADER) << std::endl;
      xgi::Utils::getPageHeader(*out, XDAQ_PAGE_HEADER);
      *out << "<div style=\"font-size:24pt;font-weight:bold\">" << title << "</div><br/>" << endl; 
      this->WebShowGenericLinks(out);
      *out << "<table width=\"100%\" height=\"4\" style=\"font-size:6pt;background:#ddeeff\" cellpadding=0 margin=0><tr width=\"*\"><td>&nbsp;</td></tr></table>" << endl;
      this->WebShowFSMControls(out);
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

  void HLXReadoutSupervisor::WebShowGenericLinks(xgi::Output * out) throw (xgi::exception::Exception) {
    
    try {
      std::string globalstatlink = toolbox::toString("/%s/Default",getApplicationDescriptor()->getURN().c_str());
      std::string settingslink = toolbox::toString("/%s/IndividualStatus",getApplicationDescriptor()->getURN().c_str());
      std::string debuglink = toolbox::toString("/%s/GlobalConfig",getApplicationDescriptor()->getURN().c_str());
      std::string restartlink = toolbox::toString("/%s/RestartCards",getApplicationDescriptor()->getURN().c_str());
      
      *out << "<div style=\"font-family:arial;font-size:10pt;color:#0000AA\">[<a href="
	   << globalstatlink << ">Global Status</a>] [<a href="
	   << settingslink << ">Settings</a>] [<a href="
	   << debuglink << ">Debugging</a>] [<a href="
	   << restartlink << ">Restart Card</a>]"
	   << "</div><br/>";
      
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

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void HLXReadoutSupervisor::WebShowModeLengthOption(xgi::Output *out,
					      std::string *title,
					      std::string *setting,
					      HCAL_HLX::u32 numTerms,
					      std::string *currentMode,
					      std::string *ModeOptions,
					      HCAL_HLX::u32 numOptions,
					      std::string *currentLength,
					      HCAL_HLX::i32 boardID) throw (xgi::exception::Exception) {
    
    try {
      string tempstring;
      std::stringstream temp;

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/ParameterSet";


      *out << "<table border=0 cellpadding=2 cellspacing=0 width=410 style=\"font-family:arial;font-size:10pt;color:#0000AA;text-align:center\">" << std::endl;
	*out << "<tr><td style=\"font-weight:bold;font-size:12pt\" colspan=2>" << *title << "</td></tr>"<< endl;

      for (u32 i=0; i<numTerms; i++){
        temp << *setting << "Mode."<<i << std::endl;
        temp >> tempstring; 
	
	*out << "<tr><td>" << endl;

        *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
	*out << cgicc::input().set("type","hidden").set("name","setting").set("value", tempstring) << std::endl;
 
        if ( boardID >= 0 ) {
	  std::stringstream tempStream;
	  std::string tempString;
	  tempStream << dec << boardID << endl; tempStream >> tempString;
	  *out << cgicc::input().set("type","hidden").set("name","board").set("value",tempString) << std::endl;	
        }
	
	*out << "<select name=\"value\">" << std::endl;	
        for (u32 j=0; j<numOptions; j++){
	  *out << "<option value=\""<<ModeOptions[j]<<"\" ";
	 if (currentMode[i] == ModeOptions[j]) *out << "selected=\"selected\" ";
	  *out << ">" << ModeOptions[j] << "</options>"<< std::endl;	
	}
	*out << "</select>" << std::endl;	

	*out << cgicc::input().set("type","submit").set("value","Update")  << std::endl;
	*out << cgicc::form() << "</td><td>" << std::endl;

/////////////////////////////////////////////
        temp << *setting << "Length."<< i << std::endl;
        temp >> tempstring; 

        *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
	*out << cgicc::input().set("type","hidden").set("name","setting").set("value", tempstring) << std::endl;

        if ( boardID >= 0 ) {
	  std::stringstream tempStream;
	  std::string tempString;
	  tempStream << dec << boardID << endl; tempStream >> tempString;
	  *out << cgicc::input().set("type","hidden").set("name","board").set("value",tempString) << std::endl;	
        }
	
	*out << cgicc::input().set("type","text").set("name","value").set("value", currentLength[i]) << std::endl;
	*out << cgicc::input().set("type","submit").set("value","Update")  << std::endl;
	*out << cgicc::form() << "</td></tr>" << std::endl;

      }
//////////////////////////////////////////////

	//*out << "<tr><td style=\"font-weight:bold;font-size:2pt\" colspan=2>&nbsp;</td></tr>" << endl;
	*out << "<tr><td>" << endl;
        temp << *setting << "NumEntries" << std::endl;
        temp >> tempstring; 

        *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
	*out << cgicc::input().set("type","hidden").set("name","setting").set("value", tempstring) << std::endl;
	*out << cgicc::input().set("type","hidden").set("name","value").set("value", "ADD") << std::endl;
 
        if ( boardID >= 0 ) {
	  std::stringstream tempStream;
	  std::string tempString;
	  tempStream << dec << boardID << endl; tempStream >> tempString;
	  *out << cgicc::input().set("type","hidden").set("name","board").set("value",tempString) << std::endl;	
        }
	
	*out << cgicc::input().set("type","submit").set("value","Add Entry")  << std::endl;
	*out << cgicc::form() << "</td><td>" << std::endl;

////////////////////////////////////////////////

        temp << *setting << "NumEntries" << std::endl;
        temp >> tempstring; 

        *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
	*out << cgicc::input().set("type","hidden").set("name","setting").set("value", tempstring) << std::endl;
	*out << cgicc::input().set("type","hidden").set("name","value").set("value", "REMOVE") << std::endl;

        if ( boardID >= 0 ) {
	  std::stringstream tempStream;
	  std::string tempString;
	  tempStream << dec << boardID << endl; tempStream >> tempString;
	  *out << cgicc::input().set("type","hidden").set("name","board").set("value",tempString) << std::endl;	
        }
	
	*out << cgicc::input().set("type","submit").set("value","Remove Last Entry")  << std::endl;
	*out << cgicc::form() << "</td></tr>" << std::endl;
        *out << "</table>" << std::endl;



    } catch(xgi::exception::Exception &e) {
      XCEPT_RETHROW(xgi::exception::Exception, "Exception caught in WebShowSwitch", e);
    }
    
 }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void HLXReadoutSupervisor::WebShowPhysicalDataOption(xgi::Output *out,
						HCAL_HLX::u16 *mFixedPatternData,
						HCAL_HLX::i32 boardID) throw (xgi::exception::Exception) {
    
    /*    try {

      string tempstring;
      std::stringstream temp;

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/ParameterSet";

	//std::string MIP[7];
	std::string electronrank[4];std::string electroncardid[4];std::string electronregionid[4];
	u16 eIsoRank[4]={0};u16 eIsoCardId[4]={0};u16 eIsoRegionId[4]={0};u16 eNonIsoRank[4]={0};u16 eNonIsoCardId[4]={0};u16 eNonIsoRegionId[4]={0};u16 MIPbits[7]={0};u16 Qbits[7]={0};u16 SFP[2][4]={{0}};

	for (int i=0; i<4;i++){
		SFP[0][i]=mFixedPatternData[i];
	}

	HLXRouting::HLXRouting tempRouting;
 	tempRouting.HLXRouting::SFPtoEMU(eIsoRank,eIsoCardId,eIsoRegionId,eNonIsoRank,eNonIsoCardId,eNonIsoRegionId,MIPbits,Qbits,SFP);

  	  temp << dec << eIsoRank[0] << endl; temp >> electronrank[0];
	  temp << dec << eIsoRank[1] << endl; temp >> electronrank[1];
	  temp << dec << eNonIsoRank[0] << endl; temp >> electronrank[2];
	  temp << dec << eNonIsoRank[1] << endl; temp >> electronrank[3];

  	  temp << dec << eIsoRegionId[0] << endl; temp >> electronregionid[0];
	  temp << dec << eIsoRegionId[1] << endl; temp >> electronregionid[1];
	  temp << dec << eNonIsoRegionId[0] << endl; temp >> electronregionid[2];
	  temp << dec << eNonIsoRegionId[1] << endl; temp >> electronregionid[3];

  	  temp << dec << eIsoCardId[0] << endl; temp >> electroncardid[0];
	  temp << dec << eIsoCardId[1] << endl; temp >> electroncardid[1];
	  temp << dec << eNonIsoCardId[0] << endl; temp >> electroncardid[2];
	  temp << dec << eNonIsoCardId[1] << endl; temp >> electroncardid[3];
 
      *out << "<table border=0 cellpadding=5 cellspacing=0 width=675 style=\"font-family:arial;font-size:10pt;color:#0000AA\">" << std::endl;
      for (int i=0; i<4; i++){
      
 	*out << "<tr><td style=\"font-weight:bold\" width=250>"<< (i<2?"Isolated ":"Non-isolated ") << (i%2) << " Electron Data:</td><td width=425>" << endl;

        *out << "<table border=0 cellpadding=0 cellspacing=0 width=425>" << std::endl;

	*out << "<tr><td>Rank (0-63)</td><td>" << std::endl;
        *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
        if ( boardID >= 0 ) { temp << dec << boardID << endl; temp >> tempstring; *out << cgicc::input().set("type","hidden").set("name","board").set("value",tempstring) << std::endl;}
        temp << "SetElectronData:Rank."<< (i+1) << std::endl; temp >> tempstring;  //new using numoption
	*out << cgicc::input().set("type","hidden").set("name","setting").set("value",tempstring) << std::endl;
	*out << cgicc::input().set("type","text").set("name","value").set("value", electronrank[i]) << std::endl;
	*out << cgicc::input().set("type","submit").set("value","Update")  << std::endl;
	*out << cgicc::form() << "</td></tr>" << std::endl;

	*out << "<tr><td>Card ID (0-6)</td><td>" << std::endl;
        *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
        if ( boardID >= 0 ) { temp << dec << boardID << endl; temp >> tempstring; *out << cgicc::input().set("type","hidden").set("name","board").set("value",tempstring) << std::endl;}
        temp << "SetElectronData:CardID."<< (i+1) << std::endl; temp >> tempstring;  //new using numoption
	*out << cgicc::input().set("type","hidden").set("name","setting").set("value",tempstring) << std::endl;
	*out << cgicc::input().set("type","text").set("name","value").set("value", electroncardid[i]) << std::endl;
	*out << cgicc::input().set("type","submit").set("value","Update")  << std::endl;
	*out << cgicc::form() << "</td></tr>" << std::endl;

	*out << "<tr><td>Region ID (0-1)</td><td>" << std::endl;
        *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
        if ( boardID >= 0 ) { temp << dec << boardID << endl; temp >> tempstring; *out << cgicc::input().set("type","hidden").set("name","board").set("value",tempstring) << std::endl;}
        temp << "SetElectronData:RegionID."<< (i+1) << std::endl; temp >> tempstring;  //new using numoption
	*out << cgicc::input().set("type","hidden").set("name","setting").set("value",tempstring) << std::endl;
	*out << cgicc::input().set("type","text").set("name","value").set("value", electronregionid[i]) << std::endl;
	*out << cgicc::input().set("type","submit").set("value","Update")  << std::endl;
	*out << cgicc::form() << "</td></tr></table>" << std::endl;

      }

      *out<< "</td></tr></table>" << std::endl;

    } catch(xgi::exception::Exception &e) {
      XCEPT_RETHROW(xgi::exception::Exception, "Exception caught in WebShowSwitch", e);
    }
    */
 }




/////////////////////////////////////////////////////////////////////////////////////////////

  void HLXReadoutSupervisor::WebShowLocalDataFiles(xgi::Output *out,
							 std::string *setting,
						     	 std::string *Directory,
						    	 std::string *FileExtension) throw (xgi::exception::Exception) {
    
    try {

bool UseFancyScript=false;
int noItems=0;

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/ParameterSet?setting=";

if (!UseFancyScript){
      cmdLink += *setting;
      cmdLink += "&value=";
}

    DIR *dp;
    struct dirent *dirp;
    dp  = opendir((*Directory).c_str());
    string FileName;

      string extension;
      std::stringstream temp;
	temp << "." << *FileExtension <<endl; temp >> extension;


    *out << "<table border=0 cellpadding=2 cellspacing=0 width=675 style=\"font-family:arial;font-size:10pt\">" << std::endl;
    *out << "<tr><td style=\"font-weight:bold;color:#0000AA;font-size:10pt\">Open File</td><td>&nbsp;</td></tr>"<<endl;

    if( dp == NULL) {
        *out << "<tr><td>Error opening " << *Directory << "</td></tr>" << endl;
    }else{
     //*out << "<tr><td>Files matching \"*" << extension << "\" in directory \"" << *Directory << "\"</td></tr>" << endl;
     *out << "<tr><td width=100>Look in:</td><td><select name=\"value\"><option value=\""<<*Directory<<"\">" << *Directory << "</options>"<<endl;
     //*out << "<option>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</option></select></td></tr>"<< std::endl;	
      
*out << "<tr><td colspan=2 align=\"center\">"<<endl;
*out<<"<table cellpadding=1 cellspacing=0 width=640 style=\"border:thin solid #7788BB;background-color:#FFFFFF\">"<<endl;
      while ((dirp = readdir(dp))!= NULL) {
           FileName = string(dirp->d_name);
		noItems++;

		if (FileName.length()>extension.length()){
		  if (FileName.substr(FileName.length()-extension.length(), extension.length() ) == extension ){
		    if (!UseFancyScript) *out << "<tr><td><a style=\"font-size:10pt;text-decoration:none;color:#000000\" href=\"" << cmdLink << FileName << "\">" << FileName << "</a></td></tr>" << endl;
		    else *out << "<tr><td><a style=\"font-size:10pt;text-decoration:none;color:#000000\" href=\"#\" onClick=\"javascript:document.getElementById('FileField').value='"  << FileName <<"'\">" << FileName << "</a></td></tr>" << endl;
		  }
		}
      }
      closedir(dp);
    }
//*out << "<tr><td>&nbsp;</td></tr>" << endl;
*out << "</table>"<<endl;
*out << "</td></tr>"<<endl;

if (UseFancyScript){
*out << "<tr><td><form method=\"GET\" action=\"" << cmdLink << "\">"<<endl;
*out << "<input type=\"hidden\" name=\"setting\" value=\"" << *setting<< "\" />"<<endl;
*out << "File name:</td><td><input type=\"text\" name=\"value\" id=\"FileField\" size=\"30\" />"<<endl;
*out << "<input type=\"submit\" value=\"Open\" />"<<endl;
*out << "</form></td></tr>";
}

*out << "<tr><td>Files of type:</td><td><select name=\"value\"><option value=\"*" << extension << "\" >" << extension << "</options>"<<endl;
//*out << "<option>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</option></select>"<<endl;
*out<<"</td></tr>"<<endl;

//*out << "</noscript>" << std::endl;

*out << "</table>"<<endl; 

    } catch(xgi::exception::Exception &e) {
      XCEPT_RETHROW(xgi::exception::Exception, "Exception caught in WebShowSwitch", e);
    }
    
 }
///////////////////////////////////////////////////////////////////////////////////////////////
