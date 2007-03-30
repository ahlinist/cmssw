#include <RecoLocalMuon/CSCRecHitC/src/CSCRecHitCProducer.h>
#include <RecoLocalMuon/CSCRecHitC/src/CSCRecHitCBuilder.h>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/MakerMacros.h>
#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <CondFormats/CSCObjects/interface/CSCGains.h>
#include <CondFormats/DataRecord/interface/CSCGainsRcd.h>
#include <CondFormats/CSCObjects/interface/CSCcrosstalk.h>
#include <CondFormats/DataRecord/interface/CSCcrosstalkRcd.h>
#include <CondFormats/CSCObjects/interface/CSCNoiseMatrix.h>
#include <CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>





// UF: 1
#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"

//#include "Utilities/Timing/interface/TimingReport.h"
// UF: 2






CSCRecHitCProducer::CSCRecHitCProducer( const edm::ParameterSet& ps ) : iev( 0 ) {
	
  stripDigiProducer_ = ps.getParameter<std::string>("CSCStripDigiProducer");
  wireDigiProducer_  = ps.getParameter<std::string>("CSCWireDigiProducer");
  useCalib           = ps.getUntrackedParameter<bool>("CSCUseCalibrations");
  debug              = ps.getUntrackedParameter<bool>("CSCDebug");
 
  recHitBuilder_     = new CSCRecHitCBuilder( ps ); // pass on the Parameter Settings

  // register what this produces
  produces<CSCRecHit2DCollection>();

}

CSCRecHitCProducer::~CSCRecHitCProducer()
{
  LogDebug("CSC") << "deleting recHitBuilder_ after " << iev << " events.";
  delete recHitBuilder_;
}


void  CSCRecHitCProducer::produce( edm::Event& ev, const edm::EventSetup& setup )
{
  LogDebug("CSC") << "[CSCRecHitCProducer::produce] start producing rechits for event " << ++iev;

	
  /*
  // find the geometry & calibrations for this event & cache it in the builder

  // Geometry
  edm::ESHandle<CSCGeometry> h;
  setup.get<MuonGeometryRecord>().get( h );
  const CSCGeometry* pgeom = &*h;
  recHitBuilder_->setGeometry( pgeom );


  // Only for data can you load in calibration constants !
  if ( useCalib ) {  
    // Strip gains
    edm::ESHandle<CSCGains> hGains;
    setup.get<CSCGainsRcd>().get( hGains );
    const CSCGains* pGains = &*hGains.product(); 
    // Strip X-talk
    edm::ESHandle<CSCcrosstalk> hCrosstalk;
    setup.get<CSCcrosstalkRcd>().get( hCrosstalk );
    const CSCcrosstalk* pCrosstalk = &*hCrosstalk.product();
    // Strip autocorrelation noise matrix
    edm::ESHandle<CSCNoiseMatrix> hNoiseMatrix;
    setup.get<CSCNoiseMatrixRcd>().get(hNoiseMatrix);
    const CSCNoiseMatrix* pNoiseMatrix = &*hNoiseMatrix.product();
    // Pass set of calibrations to builder all at once
    recHitBuilder_->setCalibration( pGains, pCrosstalk, pNoiseMatrix );
  }
  */
  






	
  // Get the collections of strip & wire digis from event
  edm::Handle<CSCStripDigiCollection> strips;
  edm::Handle<CSCWireDigiCollection> wires;
  ev.getByLabel(stripDigiProducer_, "MuonCSCStripDigi", strips);
  ev.getByLabel(wireDigiProducer_,  "MuonCSCWireDigi",  wires);


  // UF: 1
  edm::Handle<CSCALCTDigiCollection> alcts;
  edm::Handle<CSCCLCTDigiCollection> clcts;
  ev.getByLabel(stripDigiProducer_,"MuonCSCALCTDigi",alcts);
  ev.getByLabel(stripDigiProducer_,"MuonCSCCLCTDigi",clcts);
  // UF: 2


  // Create empty collection of rechits
  std::auto_ptr<CSCRecHit2DCollection> oc( new CSCRecHit2DCollection );


  // Fill the CSCRecHit2DCollection
  //recHitBuilder_->build( stripDigis.product(), wireDigis.product(), *oc);
  // UF: 1
  recHitBuilder_->build( strips.product(), 
			 wires.product(), 
			 alcts.product(), 
			 clcts.product(), 
			 *oc);
  // UF: 2






  /*
  // Put collection in event
  if (debug) std::cout << "Will output rechits collection to event" << std::endl;

  if (0)
    {
      //TimeMe t5("ProducerPut", false);
      ev.put( oc );
    }

  */









}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCRecHitCProducer);

