// Create digi collection for wires from raw data

#include <map>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <Geometry/CSCGeometry/interface/CSCLayer.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>

#include "RecoLocalMuon/CSCCalibrateDigis/src/CSCMakeWireDigiCollections.h"

using namespace std;

CSCMakeWireDigiCollections::CSCMakeWireDigiCollections(const edm::ParameterSet & p) {
  // Initialize parameters needed...

  LogDebug("CSC") << "[CSCMakeWireDigiCollections] Initialized wire collection maker";
}


CSCMakeWireDigiCollections::~CSCMakeWireDigiCollections() {

}


void CSCMakeWireDigiCollections::mkWireCollect( const CSCWireDigiCollection* wireDigis,
                                                CSCWireDigiCollection & WireDigiColl ) {

   // Loop over DetId:    
   for ( CSCWireDigiCollection::DigiRangeIterator it = wireDigis->begin(); it != wireDigis->end(); ++it ) {
           
       const CSCDetId& myDetId = (*it).first;
    
       const CSCWireDigiCollection::Range& rwire = (*it).second;

       // Only fill if not empty !!!
       if ( rwire.second - rwire.first > 0 ) WireDigiColl.put(rwire, myDetId);         
   }
}
