#include <memory>
#include "boost/shared_ptr.hpp"
#include <fstream>

#include "CondFormats/CSCObjects/interface/CSCChamberIndex.h"
#include "CondFormats/DataRecord/interface/CSCChamberIndexRcd.h"
#include "CondTools/CSC/interface/CSCChamberIndexValues.h"
#include "CondTools/CSC/interface/CSCMap1.h"

CSCChamberIndexValues::CSCChamberIndexValues(const edm::ParameterSet& iConfig)
{
  //the following line is needed to tell the framework what
  // data is being produced
  mapObj = fillChamberIndex();
  setWhatProduced(this,&CSCChamberIndexValues::produceChamberIndex);
  findingRecord<CSCChamberIndexRcd>();
  //now do what ever other initialization is needed
}


CSCChamberIndexValues::~CSCChamberIndexValues()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  delete mapObj;
}


//
// member functions
//

// ------------ method called to produce the data  ------------
CSCChamberIndexValues::ReturnType
CSCChamberIndexValues::produceChamberIndex(const CSCChamberIndexRcd& iRecord)
{
  //need a new object so to not be deleted at exit
  CSCChamberIndex* mydata=new CSCChamberIndex( *mapObj );
  return mydata;
  
}

 void CSCChamberIndexValues::setIntervalFor(const edm::eventsetup::EventSetupRecordKey &, const edm::IOVSyncValue&,
 edm::ValidityInterval & oValidity)
 {
 oValidity = edm::ValidityInterval(edm::IOVSyncValue::beginOfTime(),edm::IOVSyncValue::endOfTime());
 
 }
