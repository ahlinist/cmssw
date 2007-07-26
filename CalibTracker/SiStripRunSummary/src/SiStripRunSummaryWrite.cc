#include <iostream>

#include "CalibTracker/SiStripRunSummary/interface/DQMFlagTxt.h"
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
    oFlagTreeTxt_( "") {

  if( oFileXMLIn_.empty()) {
    throw cms::Exception( "Wrong Input parameters", 
                          "Input XML File is not specified: can not proceed.");
  }
}

void SiStripRunSummaryWrite::algoEndRun( const edm::Event      &roEVENT, const edm::EventSetup &roEVENT_SETUP){
  // Read Run Summary Flags XML file
  SerializeXML oSerializeXML;
  
  oSerializeXML.read( oFileXMLIn_.c_str());

  // Convert XML to Txt Tree
  DQMFlagTxt oDQMFlagTxt( oSerializeXML.getFlag());

  std::cout << oDQMFlagTxt << std::endl;

  // Get Txt Tree state string
  SerializeBin oSerializeBin;
  oSerializeBin.writeStr( oFlagTreeTxt_, oDQMFlagTxt);

}

SiStripRunSummary *SiStripRunSummaryWrite::getNewObject() {
  //TMP... remove it
  oFlagTreeTxt_="abcdefghilmno";
  
  SiStripRunSummary* obj=new SiStripRunSummary;
  obj->put(oFlagTreeTxt_);
  std::cout << "\n\n\n TEST REMOVE ME " << oFlagTreeTxt_ << " added value " << obj->getRunSummary() << "\n\n\n" << std::endl;
  return obj;
}
