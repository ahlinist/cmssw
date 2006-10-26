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

  if ( isData ) {
    // Pass gain constants to strip hit reconstruction package
    HitsFromStripOnly_->setCalibration( gains_ );

    // Pass X-talks and noise matrix to 2-D hit builder (these are ultimately implemented in Gatti fit)
    Make2DHits_->setCalibration( gains_, xtalk_, noise_ );
  }

  
  // Make collection of wire only hits !  
  CSCWireHitCollection woc;
  
  for ( CSCWireDigiCollection::DigiRangeIterator it = wiredc->begin(); it != wiredc->end(); ++it ){
    const CSCDetId& id = (*it).first;
    const CSCLayer* layer = getLayer( id );
    const CSCWireDigiCollection::Range rwired = wiredc->get( id );
    
    // Skip if no wire digis in this layer
    if ( rwired.second == rwired.first ) continue;
    
    std::vector<CSCWireHit> rhv = HitsFromWireOnly_->runWire( id, layer, rwired );
    
    // Add the wire hits to master collection
    woc.put( id, rhv.begin(), rhv.end() );
  }

  // Clean up the wire hit collection by trying to build wire segments
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
  
  CSCStripHitCollection soc;  
  for ( CSCStripDigiCollection::DigiRangeIterator it = stripdc->begin(); it != stripdc->end(); ++it ){
    const CSCDetId& id = (*it).first;
    const CSCLayer* layer = getLayer( id );
    const CSCStripDigiCollection::Range& rstripd = (*it).second;
    
    // Skip if no strip digis in this layer
    if ( rstripd.second == rstripd.first ) continue;
    
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
  
  int layer_idx     = 0;
  int hits_in_layer = 0;
  CSCDetId old_id; 
  CSCDetId very_old_id; 

  // Vector to store rechit within layer
  std::vector<CSCRecHit2D> hitsInLayer;
  // Initially clear it just in case...
  hitsInLayer.clear();
  
  // N.B.  I've sorted the hits from layer 1-6 always, so can test if there are "holes", that is
  // layers without hits for a given chamber.

  // Loop over strip hit collection
  for ( CSCStripHitCollection::const_iterator sit = clean_soc.begin(); sit != clean_soc.end(); ++sit ){

    if ( clean_soc.end() - clean_soc.begin() <= 0 ) continue;  // just in case...

    bool foundMatch    = false;  

    // This is the id I'll compare the wire digis against because of the ME1a confusion in data
    // i.e. ME1a == ME11 == ME1b for wire in data
    CSCDetId compId;

    const CSCStripHit& s_hit = *sit;
    const CSCDetId& sDetId   = (*sit).cscDetId();
    const CSCLayer* layer    = getLayer( sDetId );
    int shit_time            = (*sit).tmax();

    if ( layer_idx == 0 ) {
      old_id      = sDetId;
      very_old_id = old_id;
    }

    // Test if have changed layer
    // If so, store hits in previous layer in collection
    if ((sDetId.endcap()  != old_id.endcap() ) ||
        (sDetId.station() != old_id.station()) ||
        (sDetId.ring()    != old_id.ring()   ) ||
        (sDetId.chamber() != old_id.chamber()) ||
        (sDetId.layer()   != old_id.layer()  )) {

      if (hits_in_layer > 0) oc.put( old_id, hitsInLayer.begin(), hitsInLayer.end() );

      // Reset layer parameters and clear vector of rechit
      hitsInLayer.clear();
      very_old_id   = old_id;
      old_id        = sDetId;
      hits_in_layer = 0;
    }

    // For ME11, real data wire digis are labelled as belonging to ME1b, 
    // so that's where ME1a must look
    if ((      isData          ) && 
        (sDetId.station() == 1 ) && 
        (sDetId.ring()    == 4 )) {
      int sendcap  = sDetId.endcap();
      int schamber = sDetId.chamber();
      int slayer   = sDetId.layer();
      CSCDetId testId( sendcap, 1, 1, schamber, slayer );
      compId = testId;
    } else {
      compId = sDetId;
    }

    // Test if layer gap in strip hit collection --> if so, build pseudo 2-D hit from wire hit
    // N.B. as is, cannot accomodate for 2 successive gaps in layers for strip hit 
    // e.g. if no strip hit in layer 1 and 2, can only try retrieving wire hit in layer 1.

    if ((          makePseudo2DHits               ) &&
        (sDetId.layer()   == very_old_id.layer()+2) &&
        (sDetId.endcap()  == very_old_id.endcap() ) &&  
        (sDetId.station() == very_old_id.station()) &&
        (sDetId.ring()    == very_old_id.ring()   ) &&
        (sDetId.chamber() == very_old_id.chamber())) {

      if ( debug ) std::cout << " Found gap in layer for strip hit collection " << std::endl;
      CSCDetId wDetId_modified;

      // Loop over cleaned up wire hit collection
      for ( CSCWireHitCollection::const_iterator wit = clean_woc.begin(); wit != clean_woc.end(); ++wit ) {
    
        if ( clean_woc.end() - clean_woc.begin() <= 0 ) continue;

        const CSCWireHit& w_hit = *wit;   
        const CSCDetId& wDetId  = (*wit).cscDetId();
      
        if ((wDetId.endcap()  == compId.endcap()       ) &&
            (wDetId.station() == compId.station()      ) &&
            (wDetId.ring()    == compId.ring()         ) &&
            (wDetId.chamber() == compId.chamber()      ) &&
            (wDetId.layer()   == very_old_id.layer()+1 )) {

          // Because of ME-1a problem, have to do the following trick:
          int wendcap  = sDetId.endcap();
          int wstation = sDetId.station();
          int wring    = sDetId.ring();
          int wchamber = sDetId.chamber();
          int wlayer   = wDetId.layer();   
          CSCDetId testId( wendcap, wstation, wring, wchamber, wlayer );
          wDetId_modified = testId;

          const CSCLayer* w_layer = getLayer( wDetId );
          CSCRecHit2D rechit = Make2DHits_->hitFromWireOnly( testId, w_layer, w_hit);
          bool isInFiducial = Make2DHits_->isHitInFiducial( w_layer, rechit );
          if ( isInFiducial ) {
            hitsInLayer.push_back( rechit );
            hits_in_layer++;
          }
        }
      }
      if ( hits_in_layer > 0 ) {
        oc.put( wDetId_modified, hitsInLayer.begin(), hitsInLayer.end() );
        hitsInLayer.clear();
        hits_in_layer = 0;
        very_old_id = old_id;
      }      
    }


    // Loop over cleaned up wire hit collection
    for ( CSCWireHitCollection::const_iterator wit = clean_woc.begin(); wit != clean_woc.end(); ++wit ) {

      if ( clean_woc.end() - clean_woc.begin() <= 0 ) continue;

      const CSCWireHit& w_hit = *wit;
      const CSCDetId& wDetId  = (*wit).cscDetId();
      int whit_time           = (*wit).tmax();


/* Not clear to me if this is needed...
 *    // Ensure that wire and strip hit occur in same time bin +/- stripWireDeltaT
 *    if ( isData ) {
 *      int time_diff = (int) abs(whit_time - shit_time);
 *      if (time_diff <= stripWireDeltaT) continue;
 *    }     
 */

      // Again, because of ME1a, use the compId to make a comparison between strip and wire hit CSCDetId
      if ((wDetId.endcap()  == compId.endcap() ) &&
          (wDetId.station() == compId.station()) &&
          (wDetId.ring()    == compId.ring()   ) &&
          (wDetId.chamber() == compId.chamber()) &&
          (wDetId.layer()   == compId.layer()  )) { 

	CSCRecHit2D rechit = Make2DHits_->hitFromStripAndWire(sDetId, layer, w_hit, s_hit);
        bool isInFiducial = Make2DHits_->isHitInFiducial( layer, rechit );
        if ( isInFiducial ) {
          foundMatch = true;  
          hitsInLayer.push_back( rechit );
          hits_in_layer++;
        }
      }
    }
    
    // If missing wire hit in a layer, form pseudo 2-D hit from strip hit
    if ( !foundMatch && makePseudo2DHits ) { 
      if ( debug ) std::cout << " Found gap in layer for wire hit collection " << std::endl;      
      CSCRecHit2D rechit = Make2DHits_->hitFromStripOnly(sDetId, layer, s_hit);
      hits_in_layer++;
      hitsInLayer.push_back( rechit );
    }
    layer_idx++;
    very_old_id = old_id;
    old_id      = sDetId;
  }

  // Finally, after exiting loop, check that last set of hits has been added to layer
  if ( hits_in_layer > 0 ) oc.put( old_id, hitsInLayer.begin(), hitsInLayer.end() );

}


/* getLayer
 *
 */
const CSCLayer* CSCRecHitBBuilder::getLayer( const CSCDetId& detId )  {
  if ( !geom_ ) throw cms::Exception("MissingGeometry") << "[CSCRecHitBBuilder::getLayer] Missing geometry" << std::endl;
  return geom_->layer(detId);
}

