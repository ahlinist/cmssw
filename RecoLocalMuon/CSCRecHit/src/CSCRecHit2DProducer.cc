#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DProducer.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DBuilder.h>

#include <FWCore/Framework/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit1DCollection.h>

#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>

CSCRecHit2DProducer::CSCRecHit2DProducer( const edm::ParameterSet& pas ) : iev( 0 ) {
	
  stripDigiProducer_ = pas.getParameter<std::string>("CSCStripDigiProducer");
  wireDigiProducer_  = pas.getParameter<std::string>("CSCWireDigiProducer");

  recHitBuilder_ = new CSCRecHit2DBuilder( pas ); // pass on the PS

  // register what this produces
  produces<CSCRecHit2DCollection>();

  // DOMINIQUE:  want to produce also wire/strip only hits ?
  Produce1DHits  = pas.getParameter<bool>("CSCproduce1DHits");
  if ( Produce1DHits ) {
     produces<CSCRecHit1DCollection>("CSCWireHit1DCollection");
     produces<CSCRecHit1DCollection>("CSCStripHit1DCollection");
  }

}

CSCRecHit2DProducer::~CSCRecHit2DProducer()
{
  LogDebug("CSC") << "deleting recHitBuilder_ after " << iev << " events.";
  delete recHitBuilder_;
}

void  CSCRecHit2DProducer::produce( edm::Event& ev, const edm::EventSetup& setup )
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

  // DOMINIQUE: also empty collections for wire/strip only hits
  std::auto_ptr<CSCRecHit1DCollection> woc( new CSCRecHit1DCollection );
  std::auto_ptr<CSCRecHit1DCollection> soc( new CSCRecHit1DCollection );

  // fill the collection...  DOMINIQUE:  also wire/strip only collection
  recHitBuilder_->build( stripDigis.product(), wireDigis.product(), *oc, *woc, *soc ); //@@ FILL oc

  // put collection in event
  ev.put( oc );

  // DOMINIQUE: put wire/strip hit collections in event if have produced them
  if ( !Produce1DHits ) return;
  ev.put( woc, "CSCWireHit1DCollection" );
  ev.put( soc, "CSCStripHit1DCollection" );

}
