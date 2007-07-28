// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <exception>
#include <iostream>
#include <fstream>

#include "cgicc/Cgicc.h"
#include "cgicc/FormEntry.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTTPHTMLHeader.h"

#include "CalibTracker/SiStripRunSummary/interface/Flag.h"
#include "CalibTracker/SiStripRunSummary/interface/FlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/DAQFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/DCSFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/SerializeXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TECFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIDFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TriggerFlagXML.h"

const char *pcFILE_XML = "output/archive.xml";
const char *pcPAGE_URL = "http://localhost:1906/index.cgi";

Flag::State getFlagState( const int &rnVALUE);
void        processForm( cgicc::Cgicc &roCgi);
void        redirectUser();
void        setFlag( cgicc::Cgicc &roCgi, Flag *poFlag, const char *pcPREFIX);
void        showFlagTable( const char *pcPREFIX);
void        showForm();

int main( const int &rnArgc, const char *pcArgv[]) {
  try {
    // Initialize CGICC
    cgicc::Cgicc oCgi;

    std::cout << cgicc::HTTPHTMLHeader() << std::endl;

    // Head
    std::cout << cgicc::html() 
              << cgicc::head( cgicc::title( "Run Summary Flags - SiStrip")) 
              << std::endl;

    // Body
    std::cout << cgicc::body() << std::endl;

    // Check if Form was submitted
    if( oCgi.getElement( "submit") != oCgi.getElements().end()) {
      // Form submitted
      processForm( oCgi);

      // Thank user and... Thank user
      redirectUser();
    } else {
      // Form not submitted: show it
      showForm();
    }

    std::cout << cgicc::body() << cgicc::html();
  } catch( const std::exception &roEX) {
    // Error occured
  }

  return 0;
}

void showForm() {
  using namespace cgicc;
  using namespace std;

  cout << "<form>" << endl;
  cout << "  <table>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <th colspan='2' style='text-align:left'>Global</th>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <th colspan='2' style='padding-left:20px;vertical-align:top;text-align:left'>DAQ</th>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TEC</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DAQ_TEC");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TIB</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DAQ_TIB");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TID</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DAQ_TID");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TOB</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DAQ_TOB");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <th colspan='2' style='padding-left:20px;vertical-align:top;text-align:left'>DCS</th>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TEC</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DCS_TEC");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TIB</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DCS_TIB");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TID</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DCS_TID");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TOB</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DCS_TOB");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <th colspan='2' style='padding-left:20px;vertical-align:top;text-align:left'>DQM</th>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TEC</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DQM_TEC");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TIB</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DQM_TIB");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TID</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DQM_TID");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:40px;vertical-align:top'>TOB</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "DQM_TOB");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td colspan='2' style='text-align:right'>" << endl;
  cout << "        <input type='submit' name='submit' value='Submit' />" 
       << endl;
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "    <tr>" << endl;
  cout << "      <td style='padding-left:20px;vertical-align:top'>Trigger</td>" 
       << endl;
  cout << "      <td>" << endl;
  showFlagTable( "Trigger");
  cout << "      </td>" << endl;
  cout << "    </tr>" << endl;
  cout << "  </table>" << endl;
  cout << "</form>" << endl;
}

void processForm( cgicc::Cgicc &roCgi) {
  GlobalFlagXML oGlobalFlagXML;

  // DAQ
  if( FlagXML *poDAQFlagXML = oGlobalFlagXML.createChild<DAQFlagXML>()) {
    // DAQ - TEC: create Flag
    if( FlagXML *poFlag = poDAQFlagXML->createChild<TECFlagXML>()) {
      setFlag( roCgi, poFlag, "DAQ_TEC");
    }

    // DAQ - TIB: create Flag
    if( FlagXML *poFlag = poDAQFlagXML->createChild<TIBFlagXML>()) {
      setFlag( roCgi, poFlag, "DAQ_TIB");
    }

    // DAQ - TID: create Flag
    if( FlagXML *poFlag = poDAQFlagXML->createChild<TIDFlagXML>()) {
      setFlag( roCgi, poFlag, "DAQ_TID");
    }

    // DAQ - TOB: create Flag
    if( FlagXML *poFlag = poDAQFlagXML->createChild<TOBFlagXML>()) {
      setFlag( roCgi, poFlag, "DAQ_TOB");
    }
  } // End DAQ

  // DCS
  if( FlagXML *poDCSFlagXML = oGlobalFlagXML.createChild<DCSFlagXML>()) {
    // DCS - TEC: create Flag
    if( FlagXML *poFlag = poDCSFlagXML->createChild<TECFlagXML>()) {
      setFlag( roCgi, poFlag, "DCS_TEC");
    }

    // DCS - TIB: create Flag
    if( FlagXML *poFlag = poDCSFlagXML->createChild<TIBFlagXML>()) {
      setFlag( roCgi, poFlag, "DCS_TIB");
    }

    // DCS - TID: create Flag
    if( FlagXML *poFlag = poDCSFlagXML->createChild<TIDFlagXML>()) {
      setFlag( roCgi, poFlag, "DCS_TID");
    }

    // DCS - TOB: create Flag
    if( FlagXML *poFlag = poDCSFlagXML->createChild<TOBFlagXML>()) {
      setFlag( roCgi, poFlag, "DCS_TOB");
    }
  } // End DCS

  // DQM
  if( FlagXML *poDQMFlagXML = oGlobalFlagXML.createChild<DQMFlagXML>()) {
    // DQM - TEC: create Flag
    if( FlagXML *poFlag = poDQMFlagXML->createChild<TECFlagXML>()) {
      setFlag( roCgi, poFlag, "DQM_TEC");
    }

    // DQM - TIB: create Flag
    if( FlagXML *poFlag = poDQMFlagXML->createChild<TIBFlagXML>()) {
      setFlag( roCgi, poFlag, "DQM_TIB");
    }

    // DQM - TID: create Flag
    if( FlagXML *poFlag = poDQMFlagXML->createChild<TIDFlagXML>()) {
      setFlag( roCgi, poFlag, "DQM_TID");
    }

    // DQM - TOB: create Flag
    if( FlagXML *poFlag = poDQMFlagXML->createChild<TOBFlagXML>()) {
      setFlag( roCgi, poFlag, "DQM_TOB");
    }
  } // End DQM

  // Trigger
  if( FlagXML *poTriggerFlagXML = 
        oGlobalFlagXML.createChild<TriggerFlagXML>()) {

    setFlag( roCgi, poTriggerFlagXML, "Trigger");
  } // End Trigger

  std::ofstream oOut( pcFILE_XML);

  SerializeXML oSerializeXML;
  oSerializeXML.write( oOut, oGlobalFlagXML);
}

void setFlag( cgicc::Cgicc &roCgi, Flag *poFlag, const char *pcPREFIX) {
  {
    std::string oStrFlag( pcPREFIX);
    oStrFlag += "_FLAG";
    cgicc::form_iterator oIterFlag = roCgi.getElement( oStrFlag.c_str());
    if( oIterFlag != roCgi.getElements().end()) {
      poFlag->setState(
        getFlagState(
          static_cast<int>( oIterFlag->getIntegerValue() )
        ) 
      );
    } else {
      // Hmm, strange: flag was not submitted - leave it unitialized
    }
  }

  {
    std::string oStrFlag( pcPREFIX);
    oStrFlag += "_COMMENT";
    cgicc::form_iterator oIterFlag = roCgi.getElement( oStrFlag.c_str());
    if( oIterFlag != roCgi.getElements().end()) {
      poFlag->setComment( oIterFlag->getValue());
    } else {
      // Hmm, strange: flag was not submitted - leave it unitialized
    }
  }
}

Flag::State getFlagState( const int &rnVALUE) {
  switch( rnVALUE) {
    case  1: return Flag::ERROR;
    case  2: return Flag::OK;
    case  0: 
      // Fall through
    default: return Flag::UNKNOWN;
  }
}

void redirectUser() {
  using namespace std;

  cout << "Thank you for saving Run Summary Flags. You will be "
       << "redirected to the main page in a moment." << endl;
  cout << "<script language='JavaScript' type='text/javascript'>" << endl;
  cout << "setTimeout( \"location.href='"
       << pcPAGE_URL << "'\", 5000);" << endl;
  cout << "</script>" << endl;
}

void showFlagTable( const char *pcPREFIX) {
  using namespace std;

  cout << "        <table>" << endl;
  cout << "          <tr>" << endl;
  cout << "            <td>" << endl;
  cout << "              <select name='" << pcPREFIX
       << "_FLAG'>" << endl;
  cout << "                <option value='0'>Unknown</option>" << endl;
  cout << "                <option value='1'>Error</option>" << endl;
  cout << "                <option value='2'>Ok</option>" << endl;
  cout << "              </select>" << endl;
  cout << "            </td>" << endl;
  cout << "          </tr>" << endl;
  cout << "          <tr>" << endl;
  cout << "            <td><input type='text' name='" << pcPREFIX
       << "_COMMENT' /></td>" << endl;
  cout << "          </tr>" << endl;
  cout << "        </table>" << endl;
}
