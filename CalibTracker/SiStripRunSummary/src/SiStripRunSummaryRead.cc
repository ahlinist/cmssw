// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/05/07
// License: GPL

#include <iostream>

#include "CalibTracker/SiStripRunSummary/interface/SerializeTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagTxt.h"
#include "CondFormats/DataRecord/interface/SiStripRunSummaryRcd.h"
#include "CondFormats/SiStripObjects/interface/SiStripRunSummary.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "CalibTracker/SiStripRunSummary/interface/SiStripRunSummaryRead.h"

void SiStripRunSummaryRead::beginJob( const edm::EventSetup &roEVENT_SETUP) {

  edm::ESHandle<SiStripRunSummary> poESHRunSummary;
  roEVENT_SETUP.get<SiStripRunSummaryRcd>().get( poESHRunSummary);

  if( poESHRunSummary.isValid()) {
    std::istringstream oIn( poESHRunSummary->getRunSummary());

    SerializeTxt oSerializeTxt;
    if( DQMFlagTxt *poFlagTxt = oSerializeTxt.read<DQMFlagTxt>( oIn)) {
      std::cout << "Read Flag Tree" << std::endl;
      std::cout << *poFlagTxt << std::endl;
    } else {
      // Failed to read Flags Tree
      throw cms::Exception( "Run Summary Flags", 
                            "Failed to read Run Summary Flags Tree from \
string");
    }
  } else {
    // Incorrect Handle: maybe string is not in DB?
    throw cms::Exception( "Run Summary Flags", 
                          "Incorrect ESHandle<SiStripRunSummary> - not valid \
one. Are you sure Run Summary Flags Tree is in DB?");
  }
}
