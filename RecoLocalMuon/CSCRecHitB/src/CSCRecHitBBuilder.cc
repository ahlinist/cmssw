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

#include "CondFormats/CSCObjects/interface/CSCGains.h"
#include "CondFormats/DataRecord/interface/CSCGainsRcd.h"
#include "CondFormats/CSCObjects/interface/CSCcrosstalk.h"
#include "CondFormats/DataRecord/interface/CSCcrosstalkRcd.h"
#include "CondFormats/CSCObjects/interface/CSCNoiseMatrix.h"
#include "CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h"

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

  isData                 = ps.getUntrackedParameter<bool>("CSCIsRunningOnData");  
  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");
  stripWireDeltaT        = ps.getUntrackedParameter<int>("CSCstripWireDeltaTime");
  useCleanStripCollection= ps.getUntrackedParameter<bool>("CSCuseCleanStripCollection");
  useCleanWireCollection = ps.getUntrackedParameter<bool>("CSCuseCleanWireCollection");
  makePseudo2DHits       = ps.getUntrackedParameter<bool>("CSCproduce1DHits");
  
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
  CSCWireHitCollection clean_woc;  
  if ( useCleanWireCollection ) {
    // First pass geometry
    HitsFromWireSegments_->setGeometry( geom_ );
    // Now try building segments
    clean_woc = HitsFromWireSegments_->cleanWireHits(woc);
  } else {
    clean_woc = woc;
  }

  
  // Make collection of strip only hits
  
  // Pass gain constants if it is data
  if (isData) HitsFromStripOnly_->setCalibration( gains_ );

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
  CSCStripHitCollection clean_soc;
  if ( useCleanStripCollection ) {
    // First pass geometry
    HitsFromStripSegments_->setGeometry( geom_ );
    clean_soc = HitsFromStripSegments_->cleanStripHits(soc);
  } else {
    clean_soc = soc;
  }

  
  // Now create 2-D hits by looking at superposition of strip and wire hit

  // Pass X-talks and noise matrix which are implemented in Gatti fit
  if (isData) Make2DHits_->setCalibration( xtalk_, noise_ );
  
  int layer_idx     = 0;
  int hits_in_layer = 0;
  std::vector<CSCRecHit2D> hitsInLayer;
  // Clear vector of rechit (useful if have problem in buidling 2-D hits...
  hitsInLayer.clear();

  CSCDetId old_id; 
  
  // N.B.  I've sorted the hits from layer 1-6 always, so can test if there are "holes", that is
  // layers without hits for a given chamber.

  // Loop over strip hit collection
  for ( CSCStripHitCollection::const_iterator sit = clean_soc.begin(); sit != clean_soc.end(); ++sit ){

//    if ( clean_soc.end() - clean_soc.begin() <= 0 ) break;

    const CSCStripHit& s_hit = *sit;
    const CSCDetId& sDetId   = (*sit).cscDetId();
    const CSCLayer* layer    = getLayer( sDetId );
    int shit_time            = (*sit).tmax();
    
    bool foundMatch = false;


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
        

    // Loop over cleaned up wire hit collection
    for ( CSCWireHitCollection::const_iterator wit = clean_woc.begin(); wit != clean_woc.end(); ++wit ) {

//    if ( clean_woc.end() - clean_woc.begin() <= 0 ) break;

      const CSCWireHit& w_hit = *wit;
      const CSCDetId& wDetId  = (*wit).cscDetId();
      int whit_time           = (*wit).tmax();

      if (isData) {
        int time_diff = (int) abs(whit_time - shit_time);
      	if (time_diff <= stripWireDeltaT) continue;
      }     

      if ((wDetId.endcap()  == sDetId.endcap() ) &&
	  (wDetId.station() == sDetId.station()) &&
	  (wDetId.ring()    == sDetId.ring()   ) &&
	  (wDetId.chamber() == sDetId.chamber()) &&
	  (wDetId.layer()   == sDetId.layer()  )) { 
	foundMatch = true;  
	hits_in_layer++;
	CSCRecHit2D rechit = Make2DHits_->hitFromStripAndWire(wDetId, layer, w_hit, s_hit);
	hitsInLayer.push_back( rechit );
      }
    }
    
    // If missing wire hit in a layer, form pseudo 2-D hit from strip hit.
    if ( !foundMatch && makePseudo2DHits ) { 
      CSCRecHit2D rechit = Make2DHits_->hitFromStripOnly(sDetId, layer, s_hit);
      hits_in_layer++;
      hitsInLayer.push_back( rechit );
    }
    
    layer_idx++;
    old_id = sDetId;
  }
  // Add last set of hits to collection if found hits
  if ( hits_in_layer > 0 ) oc.put( old_id, hitsInLayer.begin(), hitsInLayer.end() );
}



/* getLayer
 *
 */
const CSCLayer* CSCRecHitBBuilder::getLayer( const CSCDetId& detId )  {
  if ( !geom_ ) throw cms::Exception("MissingGeometry") << "[CSCRecHitBBuilder::getLayer] Missing geometry" << std::endl;
  return geom_->layer(detId);
}

