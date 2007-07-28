// Author : Samvel Khalatian (samvel at fnal dot gov) 
// Created: 06/29/07 
// Licence: GPL 

#include <fstream>
#include <sstream>

#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagXML.h"
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

void SiStripRunSummaryWrite::algoEndJob() {
  std::ifstream oFileIn( oFileXMLIn_.c_str());

  // Read Run Summary Flags XML file
  SerializeXML oSerializeXML;
  if( GlobalFlagXML *poFlagXML = oSerializeXML.read<GlobalFlagXML>( oFileIn)) {
    // Convert XML to Txt Tree
    GlobalFlagTxt oGlobalFlagTxt( poFlagXML);

    // Get Txt Tree state string
    std::ostringstream oOut;

    SerializeTxt oSerializeTxt;
    if( !oSerializeTxt.write( oOut, oGlobalFlagTxt)) {
      // Failed to serialize tree
      throw cms::Exception( "Run Summary Flags", 
                            "Failed to serialize Run Summary Flags Tree in \
string");
    }

    oFlagTreeTxt_ = oOut.str();
  } else {
    // Failed to read Flags Tree from file
    throw cms::Exception( "Run Summary Flags", 
                          "Failed to read Run Summary Flags Tree from XML \
File");
  }
}

SiStripRunSummary *SiStripRunSummaryWrite::getNewObject() {
  SiStripRunSummary *poRunSummary = new SiStripRunSummary;
  poRunSummary->put( oFlagTreeTxt_);

  return poRunSummary;
}
