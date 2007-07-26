// Author : Samvel Khalatian (samvel at fnal dot gov) 
// Created: 06/29/07 
// Licence: GPL 

#include <fstream>
#include <sstream>

#include "CalibTracker/SiStripRunSummary/interface/DQMFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/SerializeTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/SerializeXML.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CalibTracker/SiStripRunSummary/interface/SiStripRunSummaryWrite.h"

SiStripRunSummaryWrite::SiStripRunSummaryWrite( 
  const edm::ParameterSet &roCONFIG)
  : ConditionDBWriter<SiStripRunSummary>( roCONFIG),
    oFileXMLIn_( roCONFIG.getUntrackedParameter<std::string>( 
      "oFileXMLIn",
      "") ),
    oFlagTreeTxt_( "") 
{

  if( oFileXMLIn_.empty()) {
    throw cms::Exception( "Wrong Input parameters", 
                          "Input XML File is not specified: can not proceed.");
  }

  // [ToDo: Add file check for existance]
}

void SiStripRunSummaryWrite::algoEndRun( const edm::Event      &roEVENT, 
                                         const edm::EventSetup &roEVENT_SETUP) {

  std::ifstream oFileIn( oFileXMLIn_.c_str());

  // Read Run Summary Flags XML file
  SerializeXML oSerializeXML;
  if( DQMFlagXML *poFlagXML = oSerializeXML.read<DQMFlagXML>( oFileIn)) {
    // Convert XML to Txt Tree
    DQMFlagTxt oDQMFlagTxt( poFlagXML);

    // Get Txt Tree state string
    std::ostringstream oOut( oFlagTreeTxt_);

    SerializeTxt oSerialize;
    if( !oSerialize.write( oOut, oDQMFlagTxt)) {
      // Failed to serialize tree
      throw cms::Exception( "Run Summary Flags", 
                            "Failed to serialize Run Summary Flags Tree in string");
    }
  } else {
    // Failed to read Flags Tree from file
    throw cms::Exception( "Run Summary Flags", 
                          "Failed to read Run Summary Flags Tree from XML File");
  }
}

SiStripRunSummary *SiStripRunSummaryWrite::getNewObject() {
  SiStripRunSummary *poRunSummary = new SiStripRunSummary;
  poRunSummary->put( oFlagTreeTxt_);

  return poRunSummary;
}
