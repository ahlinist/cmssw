// This is CSCRecHitBBuilder.cc

#include <RecoLocalMuon/CSCRecHitB/src/CSCRecHitBBuilder.h>

#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>
#include <DataFormats/CSCRecHit/interface/CSCWireHitCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCStripHitCollection.h>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <RecoLocalMuon/CSCRecHitB/src/CSCHitFromStripOnly.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCHitFromWireOnly.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCWireSegments.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripSegments.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCMake2DRecHit.h>
#include <iostream>


CSCRecHitBBuilder::CSCRecHitBBuilder( const edm::ParameterSet& ps ) : geom_(0) {
  
  // Receives ParameterSet percolated down from EDProducer	
  
  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");
  stripWireDeltaT        = ps.getUntrackedParameter<int>("CSCstripWireDeltaTime");
  
  HitsFromStripOnly_     = new CSCHitFromStripOnly( ps ); 
  HitsFromWireOnly_      = new CSCHitFromWireOnly( ps );  
  HitsFromWireSegments_  = new CSCWireSegments( ps );
  HitsFromStripSegments_ = new CSCStripSegments( ps );
  Make2DHits_            = new CSCMake2DRecHit( ps );
}


CSCRecHitBBuilder::~CSCRecHitBBuilder() {}


/* Member function build
 *
 */
void CSCRecHitBBuilder::build( const CSCStripDigiCollection* stripdc, const CSCWireDigiCollection* wiredc,
                               CSCRecHit2DCollection& oc ) {
  
  // Make collection of wire only hits !
  
  if (debug) std::cout << "[CSCRecHitBBuilder] Now producing wire hits" << std::endl;
  CSCWireHitCollection woc;
  
  for ( CSCWireDigiCollection::DigiRangeIterator it = wiredc->begin(); it != wiredc->end(); ++it ){
    const CSCDetId& id = (*it).first;
    const CSCLayer* layer = getLayer( id );
    const CSCWireDigiCollection::Range rwired = wiredc->get( id );
    
    // Skip if no wire digis in this layer
    if ( rwired.second == rwired.first ) continue;
    
    // This is running CSCRecHit1DFromWireOnly.cc
    std::vector<CSCWireHit> rhv = HitsFromWireOnly_->runWire( id, layer, rwired );
    
    // Add the wire hits to master collection
    woc.put( id, rhv.begin(), rhv.end() );
  }
  
  // Clean up the wire hit collection by trying to build segments
  if (debug) std::cout << "[CSCRecHitFrom1DHitsBuilder] Now trying to create wire segments to clean up wire hit collection" << std::endl;
  // First pass geometry
  HitsFromWireSegments_->setGeometry( geom_ );
  // Now try building segments
  CSCWireHitCollection clean_woc = HitsFromWireSegments_->cleanWireHits(woc);
  
  
  
  // Make collection of strip only hits
  
  if (debug) std::cout << "[CSCRecHitBBuilder] Now producing strip hits" << std::endl;
  CSCStripHitCollection soc;
  
  for ( CSCStripDigiCollection::DigiRangeIterator it = stripdc->begin(); it != stripdc->end(); ++it ){
    const CSCDetId& id = (*it).first;
    const CSCLayer* layer = getLayer( id );
    const CSCStripDigiCollection::Range& rstripd = (*it).second;
    
    // Skip if no strip digis in this layer
    if ( rstripd.second == rstripd.first ) continue;
    
    // This is running CSCRecHit1DFromStripOnly.cc
    std::vector<CSCStripHit> rhv = HitsFromStripOnly_->runStrip( id, layer, rstripd );
    
    // Add the strip hits to master collection
    soc.put( id, rhv.begin(), rhv.end() );
  }
  
  // Now clean up the strip hit collection by trying to build strip segments
  if (debug) std::cout << "[CSCRecHitBBuilder] Now trying to create strip segments to clean up strip hit collection" << std::endl;
  // First pass geometry
  HitsFromStripSegments_->setGeometry( geom_ );
  CSCStripHitCollection clean_soc = HitsFromStripSegments_->cleanStripHits(soc);
  

  

  // Now create 2-D hits by looking at superposition of strip and wire hit

  if (debug) std::cout << "[CSCRecHitBBuilder] Now producing 2D hits" << std::endl;
  
  int layer_idx = 0;
  int hits_in_layer = 0;
  std::vector<CSCRecHit2D> hitsInLayer;
  CSCDetId old_id; 
  

  // Loop over strip hit collection
  for ( CSCStripHitCollection::const_iterator sit = clean_soc.begin(); sit != clean_soc.end(); ++sit ){
      
    const CSCStripHit& s_hit = *sit;
    const CSCDetId& sDetId = (*sit).cscDetId();
    const CSCLayer* layer = getLayer( sDetId );
    int shit_time = (*sit).tmax();
    
    if ( layer_idx == 0 ) old_id = sDetId;
    
    if ((sDetId.endcap()  != old_id.endcap() ) ||
	(sDetId.station() != old_id.station()) ||
	(sDetId.ring()    != old_id.ring()   ) ||
	(sDetId.chamber() != old_id.chamber()) ||
	(sDetId.layer()   != old_id.layer()  )) {
      oc.put( old_id, hitsInLayer.begin(), hitsInLayer.end() );
      // Reset layer parameters and clear vector of rechit
      hitsInLayer.clear();
      old_id = sDetId;
      hits_in_layer = 0;
    }
    
    bool foundMatch = false;
    

    // Loop over cleaned up wire hit collection
    for ( CSCWireHitCollection::const_iterator wit = clean_woc.begin(); wit != clean_woc.end(); ++wit ) {
    
      const CSCWireHit& w_hit = *wit;
      const CSCDetId& wDetId = (*wit).cscDetId();
      int whit_time = (*wit).tmax();

      
      int time_diff = (int) abs(whit_time - shit_time);
      
      if ((wDetId.endcap()  == sDetId.endcap() ) &&
	  (wDetId.station() == sDetId.station()) &&
	  (wDetId.ring()    == sDetId.ring()   ) &&
	  (wDetId.chamber() == sDetId.chamber()) &&
	  (wDetId.layer()   == sDetId.layer()  )) { 
	// &&           (time_diff        <= stripWireDeltaT)) {     // Found problem in wire timing, so don't use this for now.
	foundMatch = true;  
	hits_in_layer++;
	CSCRecHit2D rechit = Make2DHits_->hitFromStripAndWire(wDetId, layer, w_hit, s_hit);
	hitsInLayer.push_back( rechit );
      }
    }
    
    // If missing strip hit in a layer, form pseudo 2-D hit from strip hit.
    
    /*  Here is where you can build 1-D hits !!!   --> ignore for now until we have validated 2-D hits
     *
     *       if ( !foundMatch ) { 
     *           CSCRecHit2D rechit = Make2DHits_->hitFromStripOnly(sDetId, layer, w_hit);
     *           hits_in_layer++;
     *           hitsInLayer.push_back( rechit );
     *       }
     */
    
    layer_idx++;
    old_id = sDetId;
  }
  // Add last set of hits to collection if found hits
  if ( hits_in_layer > 0 ) oc.put( old_id, hitsInLayer.begin(), hitsInLayer.end() );
}



/* setGeometry
 *
 */
void CSCRecHitBBuilder::setGeometry( const CSCGeometry* geom ) {
  geom_ = geom;
}


/* getLayer
 *
 */
const CSCLayer* CSCRecHitBBuilder::getLayer( const CSCDetId& detId )  {
  if ( !geom_ ) throw cms::Exception("MissingGeometry") << "[CSCRecHitBBuilder::getLayer] Missing geometry" << std::endl;
  return geom_->layer(detId);
}

