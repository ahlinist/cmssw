#include <RecoLocalMuon/CSCRecHitB/src/CSCRecHitBProducer.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCRecHitBBuilder.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripGainAvg.h>

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

#include <DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h>


CSCRecHitBProducer::CSCRecHitBProducer( const edm::ParameterSet& ps ) : iRun( 0 ), CSCstripGainAvg( 1 ) {
	
  stripDigiProducer_ = ps.getParameter<std::string>("CSCStripDigiProducer");
  wireDigiProducer_  = ps.getParameter<std::string>("CSCWireDigiProducer");
  useCalib           = ps.getUntrackedParameter<bool>("CSCUseCalibrations");
  debug              = ps.getUntrackedParameter<bool>("CSCDebug");
 
  recHitBuilder_     = new CSCRecHitBBuilder( ps ); // pass on the Parameter Settings
  stripGainAvg_      = new CSCStripGainAvg( ps ); // pass on the Parameter Settings

  // register what this produces
  produces<CSCRecHit2DCollection>();

}

CSCRecHitBProducer::~CSCRecHitBProducer()
{
  delete recHitBuilder_;
}


void  CSCRecHitBProducer::produce( edm::Event& ev, const edm::EventSetup& setup )
{

	
  // find the geometry & calibrations for this event & cache it in the builder

  // Geometry
  edm::ESHandle<CSCGeometry> h;
  setup.get<MuonGeometryRecord>().get( h );
  const CSCGeometry* pgeom = &*h;
  recHitBuilder_->setGeometry( pgeom );

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
    stripGainAvg_->setCalibration( pGains );
    if (ev.id().run() != iRun) {
      CSCstripGainAvg = stripGainAvg_->getStripGainAvg();
      iRun = ev.id().run();
    }
    recHitBuilder_->setCalibration( CSCstripGainAvg, pGains, pCrosstalk, pNoiseMatrix );
  }
	
  // Get the collections of strip & wire digis from event
  edm::Handle<CSCStripDigiCollection> stripDigis;
  edm::Handle<CSCWireDigiCollection> wireDigis;
  ev.getByLabel(stripDigiProducer_, "MuonCSCStripDigi", stripDigis);
  ev.getByLabel(wireDigiProducer_,  "MuonCSCWireDigi",  wireDigis);

  // Get the collections of CLCTs and ALCTs
  edm::Handle<CSCALCTDigiCollection> alcts;
  edm::Handle<CSCCLCTDigiCollection> clcts;
  try {
    ev.getByLabel(stripDigiProducer_,"MuonCSCALCTDigi",alcts);
    ev.getByLabel(stripDigiProducer_,"MuonCSCCLCTDigi",clcts);
  }
  catch ( ... ) {
//    edm::LogError("CSCRecHitBProducer") << "Warning: L1 trigger info not available " ;
  }

  // Create empty collection of rechits
  std::auto_ptr<CSCRecHit2DCollection> oc( new CSCRecHit2DCollection );


  // Fill the CSCRecHit2DCollection
  recHitBuilder_->build( stripDigis.product(), wireDigis.product(), 
                         alcts.product(), clcts.product(),*oc);


  // Put collection in event
  if (debug) std::cout << "Will output rechits collection to event" << std::endl;
  ev.put( oc );

}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCRecHitBProducer);

