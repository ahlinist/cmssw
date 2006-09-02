#include <RecoLocalMuon/CSCRecHitB/src/CSCRecHitBProducer.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCRecHitBBuilder.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include <FWCore/Framework/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
//#include <DataFormats/CSCRecHit/interface/CSCWireHitCollection.h>
//#include <DataFormats/CSCRecHit/interface/CSCStripHitCollection.h>

#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>

CSCRecHitBProducer::CSCRecHitBProducer( const edm::ParameterSet& ps ) : iev( 0 ) {
	
  stripDigiProducer_ = ps.getParameter<std::string>("CSCStripDigiProducer");
  wireDigiProducer_  = ps.getParameter<std::string>("CSCWireDigiProducer");

  recHitBuilder_ = new CSCRecHitBBuilder( ps ); // pass on the Parameter Settings

  // register what this produces
  produces<CSCRecHit2DCollection>();

}

CSCRecHitBProducer::~CSCRecHitBProducer()
{
  LogDebug("CSC") << "deleting recHitBuilder_ after " << iev << " events.";
  delete recHitBuilder_;
}


void  CSCRecHitBProducer::produce( edm::Event& ev, const edm::EventSetup& setup )
{
  LogDebug("CSC") << "start producing rechits for event " << ++iev;
	
  // find the geometry (& conditions?) for this event & cache it in the builder
  edm::ESHandle<CSCGeometry> h;
  setup.get<MuonGeometryRecord>().get( h );
  const CSCGeometry* pgeom = &*h;
  recHitBuilder_->setGeometry( pgeom );
	
  // get the collections of strip & wire digis from event
  edm::Handle<CSCStripDigiCollection> stripDigis;
  edm::Handle<CSCWireDigiCollection> wireDigis;
  ev.getByLabel(stripDigiProducer_, "MuonCSCStripDigi", stripDigis);
  ev.getByLabel(wireDigiProducer_,  "MuonCSCWireDigi",  wireDigis);

  // create empty collection of rechits
  std::auto_ptr<CSCRecHit2DCollection> oc( new CSCRecHit2DCollection );

  // Empty collections for wire/strip only hits 
//  std::auto_ptr<CSCStripHitCollection> soc( new CSCStripHitCollection );


  // fill the collection
  recHitBuilder_->build( stripDigis.product(), wireDigis.product(), *oc);

  // put collection in event
  ev.put( oc );

}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCRecHitBProducer)

