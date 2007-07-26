#include <string>
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/SiStripRunSummaryRcd.h"
#include "CondFormats/SiStripObjects/interface/SiStripRunSummary.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// #include "CalibTracker/SiStripRunSummary/interface/SerializeTxt.h"

#include "CalibTracker/SiStripRunSummary/interface/SiStripRunSummaryRead.h"
#include <iostream>

SiStripRunSummaryRead::SiStripRunSummaryRead( 
  const edm::ParameterSet &roCONFIG) {}

void SiStripRunSummaryRead::beginJob( const edm::EventSetup &roEVENT_SETUP) {



  //SiStripRunSummary oRecord( roEVENT_SETUP.get<SiStripRunSummary>()); 


  std::cout <<"HERE I AM"<<std::endl;
  edm::ESHandle<SiStripRunSummary> pDD;
  roEVENT_SETUP.get<SiStripRunSummaryRcd>().get(pDD );

  std::cout << pDD->getRunSummary();


}
