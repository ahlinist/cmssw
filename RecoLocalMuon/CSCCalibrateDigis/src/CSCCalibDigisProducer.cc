// You may need to checkout and compile CondFormats/CSCObjects first for this to run.

// system include files
#include <memory>
//#include <stdio.h>
//#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include <FWCore/Framework/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include "CondFormats/CSCObjects/interface/CSCGains.h"   
#include "CondFormats/DataRecord/interface/CSCGainsRcd.h"

#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include "RecoLocalMuon/CSCCalibrateDigis/src/CSCCalibDigisProducer.h"
#include "RecoLocalMuon/CSCCalibrateDigis/src/CSCMakeStripDigiCollections.h"
#include "RecoLocalMuon/CSCCalibrateDigis/src/CSCMakeWireDigiCollections.h"

using namespace std;

// Constructor:
CSCCalibDigisProducer::CSCCalibDigisProducer(const edm::ParameterSet& pset) : iev( 0 ){

  // Reading in parameters from .cfg file:
  RawDataLabel       = pset.getUntrackedParameter<string>("whichRawData");


  // Pass on the .cfg parameters to other classes
  mkStripCollect_   = new CSCMakeStripDigiCollections( pset );
  mkWireCollect_    = new CSCMakeWireDigiCollections( pset );

  // Register what are the collections produced
  produces<CSCWireDigiCollection>("MuonCSCWireDigi");
  produces<CSCStripDigiCollection>("MuonCSCStripDigi");

  LogDebug("CSC") << "[CSCCalibDigisProducer] Construction completed" << "\n";

}

// Destructor:
CSCCalibDigisProducer::~CSCCalibDigisProducer()
{
  LogDebug("CSC") << "[CSCCalibDigisProducer] Destructor called at event " << iev << "\n";

  // Free memory:
  delete mkStripCollect_;
  delete mkWireCollect_;
}


void CSCCalibDigisProducer::produce(edm::Event& ev, const edm::EventSetup& evSetup)
{
  using namespace edm;

// The following parameters only needs to be set once
  if (iev == 0) {
    LogDebug("CSC") << "[CSCCalibDigisProducer] Now initializing parameters for event " << ++iev << "\n";

    // Get the gains and compute global gain average to store for later use in strip calibration
    edm::ESHandle<CSCGains> hGains;
    evSetup.get<CSCGainsRcd>().get( hGains );
    const CSCGains* pGains = &*hGains.product(); 
    mkStripCollect_->setGains( pGains );
    mkStripCollect_->getStripGainAvg();

    LogDebug("CSC") << "[CSCCalibDigisProducer] I was able to read in the CSC gains !!!" << "\n";
  }

  // Get the CSC Digis :
  // Get collection of strip digis from event
  Handle<CSCStripDigiCollection> stripDigis;
  ev.getByLabel(RawDataLabel, "MuonCSCStripDigi", stripDigis);
  
  // Get the collection of wire digis from event
  Handle<CSCWireDigiCollection> wireDigis;
  ev.getByLabel(RawDataLabel,  "MuonCSCWireDigi",  wireDigis);

  // Create empty collections of digis to fill
  std::auto_ptr<CSCWireDigiCollection> calibWireDigis( new CSCWireDigiCollection() );
  std::auto_ptr<CSCStripDigiCollection> calibStripDigis( new CSCStripDigiCollection() );

  // Create wire and strip collections from raw data:  
  LogDebug("CSC") << "[CSCCalibDigisProducer] Making the CSC strip digi collections" << "\n";
  mkStripCollect_->mkStripCollect( stripDigis.product(), *calibStripDigis);

  LogDebug("CSC") << endl << "[CSCCalibDigisProducer] Making the CSC wire digi collections" << "\n";
  mkWireCollect_->mkWireCollect( wireDigis.product(), *calibWireDigis);


  // Put calibrated digi collections in event
  ev.put(calibStripDigis, "MuonCSCStripDigi");
  ev.put(calibWireDigis, "MuonCSCWireDigi");

  iev++;
}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCCalibDigisProducer)
