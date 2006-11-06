// This is CSCRecHitBBuilder.cc

#include <RecoLocalMuon/CSCRecHitB/src/CSCRecHitBBuilder.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCHitFromStripOnly.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCHitFromWireOnly.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCWireSegments.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripSegments.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCMake2DRecHit.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCWireHitCollection.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCStripHitCollection.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCRangeMapAccessor.h>

#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>

#include <CondFormats/CSCObjects/interface/CSCGains.h>
#include <CondFormats/DataRecord/interface/CSCGainsRcd.h>
#include <CondFormats/CSCObjects/interface/CSCcrosstalk.h>
#include <CondFormats/DataRecord/interface/CSCcrosstalkRcd.h>
#include <CondFormats/CSCObjects/interface/CSCNoiseMatrix.h>
#include <CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <iostream>


/* Constructor
 *
 */
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

/* Destructor
 *
 */
CSCRecHitBBuilder::~CSCRecHitBBuilder() {
  delete HitsFromStripOnly_;
  delete HitsFromWireOnly_;
  delete HitsFromWireSegments_;
  delete HitsFromStripSegments_;
  delete Make2DHits_;   
}


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


  // Sort clean hit collections by layer    

  if (debug) std::cout << "[CSCRecHitBBuilder]  sorting hits by layer" << std::endl;
  std::vector<CSCDetId> stripLayer;
  std::vector<CSCDetId>::const_iterator sIt;
  std::vector<CSCDetId> wireLayer;
  std::vector<CSCDetId>::const_iterator wIt;
 
  // Sort strip hits  
  for ( CSCStripHitCollection::const_iterator it = clean_soc.begin(); it != clean_soc.end(); it++ ) {
    bool insert = true;
    for ( sIt=stripLayer.begin(); sIt != stripLayer.end(); ++sIt ) {
      if (((*it).cscDetId().chamber() == (*sIt).chamber()) &&
	  ((*it).cscDetId().station() == (*sIt).station()) &&
	  ((*it).cscDetId().ring()    == (*sIt).ring())    &&
	  ((*it).cscDetId().endcap()  == (*sIt).endcap())  &&
	  ((*it).cscDetId().layer()   == (*sIt).layer()))
	insert = false;
    }
    if ( insert ) stripLayer.push_back((*it).cscDetId());
  }
  if (debug) std::cout << "[CSCRecHitBBuilder] Done sorting strip hits" << std::endl;
  if (debug) std::cout << "# of layers with strip hits: " << stripLayer.size() << std::endl;

  // Sort wire hits
  for ( CSCWireHitCollection::const_iterator it = clean_woc.begin(); it != clean_woc.end(); it++ ) {
    bool insert = true;
    for ( wIt=wireLayer.begin(); wIt != wireLayer.end(); ++wIt ) {
      if (((*it).cscDetId().chamber() == (*wIt).chamber()) &&
          ((*it).cscDetId().station() == (*wIt).station()) &&
          ((*it).cscDetId().ring()    == (*wIt).ring())    &&
          ((*it).cscDetId().endcap()  == (*wIt).endcap())  &&
          ((*it).cscDetId().layer()   == (*wIt).layer()))
        insert = false;
    }
    if ( insert ) wireLayer.push_back((*it).cscDetId());
  }
  if (debug) std::cout << "[CSCRecHitBBuilder] Done sorting wire hits" << std::endl;
  if (debug) std::cout << "# of layers with wire hits: " << wireLayer.size() << std::endl;


  // Now create 2-D hits by looking at superposition of strip and wire hit in a layer
  //
  // N.B.  I've sorted the hits from layer 1-6 always, so can test if there are "holes", 
  // that is layers without hits for a given chamber.

  int n_rechit = 0;

  // Vector to store rechit within layer
  std::vector<CSCRecHit2D> hitsInLayer;

  if (debug) std::cout << "[CSCRecHitBBuilder] Now trying to build 2D hits" << std::endl;

  int layer_idx     = 0;
  int hits_in_layer = 0;
  CSCDetId old_id; 

  // Now loop over each layer containing strip hits
  for ( sIt=stripLayer.begin(); sIt != stripLayer.end(); ++sIt ) {

    bool foundMatch = false;
    hitsInLayer.clear();
    hits_in_layer = 0;
   
    std::vector<CSCStripHit> cscStripHit;
    
    CSCRangeMapAccessor acc;
    CSCStripHitCollection::range range = clean_soc.get(acc.cscDetLayer(*sIt));

    // Create vector of strip hits for this layer    
    for ( CSCStripHitCollection::const_iterator clean_soc = range.first; clean_soc != range.second; clean_soc++)
      cscStripHit.push_back(*clean_soc);

    const CSCDetId& sDetId = (*sIt);
    const CSCLayer* layer  = getLayer( sDetId );

    // This is used to test for gaps in layers and needs to be initialized here 
    if ( layer_idx == 0 ) {
      old_id = sDetId;
    }

    // This is the id I'll compare the wire digis against because of the ME1a confusion in data
    // i.e. ME1a == ME11 == ME1b for wire in data
    CSCDetId compId;

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

    if ((          makePseudo2DHits          ) &&
        (sDetId.layer()   == old_id.layer()+2) &&
        (sDetId.endcap()  == old_id.endcap() ) &&  
        (sDetId.station() == old_id.station()) &&
        (sDetId.ring()    == old_id.ring()   ) &&
        (sDetId.chamber() == old_id.chamber())) {

      if ( debug ) std::cout << " Found gap in layer for strip hit collection " << std::endl;
      CSCDetId wDetId_modified;

      // Then loop over each layer containing wire hits and try
      // to see if have wire hits in gap
      for ( wIt=wireLayer.begin(); wIt != wireLayer.end(); ++wIt ) {
        
        const CSCDetId wDetId = (*wIt);
      
        if ((wDetId.endcap()  == compId.endcap() ) &&
            (wDetId.station() == compId.station()) &&
            (wDetId.ring()    == compId.ring()   ) &&
            (wDetId.chamber() == compId.chamber()) &&
            (wDetId.layer()   == compId.layer()  )) {

          // Create vector of wire hits for this layer
          std::vector<CSCWireHit> cscWireHit;   
          CSCRangeMapAccessor acc2;
          CSCWireHitCollection::range range = clean_woc.get(acc2.cscDetLayer(*wIt));
        
          for ( CSCWireHitCollection::const_iterator clean_woc = range.first; clean_woc != range.second; clean_woc++)
            cscWireHit.push_back(*clean_woc);

          if ( debug ) std::cout << "# wire hits in layer: "<< cscWireHit.size() << std::endl;

          // Because of ME-1a problem, have to do the following trick:
          int wendcap  = sDetId.endcap();
          int wstation = sDetId.station();
          int wring    = sDetId.ring();
          int wchamber = sDetId.chamber();
          int wlayer   = wDetId.layer();   
          CSCDetId testId( wendcap, wstation, wring, wchamber, wlayer );
          wDetId_modified = testId;

          const CSCLayer* w_layer = getLayer( wDetId );
  
          // Build pseudo 2D hit for all wire hits within this layer
          for ( unsigned j = 0; j != cscWireHit.size(); j++ ) {
            const CSCWireHit w_hit = cscWireHit[j];
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
          if ( debug ) std::cout << "# of rechit/ layer = " << hits_in_layer << std::endl;
          hits_in_layer = 0;
          old_id = wDetId_modified;
        }      
      }
    }

    // Now try to build proper 2D rechit from overlap of strip/wire hits


    for ( wIt=wireLayer.begin(); wIt != wireLayer.end(); ++wIt ) {
        
      const CSCDetId& wDetId  = (*wIt);
        
      // Again, because of ME1a, use the compId to make a comparison between strip and wire hit CSCDetId
      if ((wDetId.endcap()  == compId.endcap() ) &&
          (wDetId.station() == compId.station()) &&
          (wDetId.ring()    == compId.ring()   ) &&
          (wDetId.chamber() == compId.chamber()) &&
          (wDetId.layer()   == compId.layer()  )) {
          
        // Create vector of wire hits for this layer
        std::vector<CSCWireHit> cscWireHit;
       
        CSCRangeMapAccessor acc2;
        CSCWireHitCollection::range range = clean_woc.get(acc2.cscDetLayer(*wIt));
      
        for ( CSCWireHitCollection::const_iterator clean_woc = range.first; clean_woc != range.second; clean_woc++)
          cscWireHit.push_back(*clean_woc);

        // Build 2D hit for all possible strip-wire pairs 
        // overlapping within this layer
        if (debug) std::cout << "# strip hits in layer: " << cscStripHit.size() << "  " 
                             << "# wire hits in layer: "  << cscWireHit.size()  << std::endl;

        for (unsigned i = 0; i != cscStripHit.size(); i++ ) {
          const CSCStripHit s_hit = cscStripHit[i];
          for (unsigned j = 0; j != cscWireHit.size(); j++ ) {
            const CSCWireHit w_hit = cscWireHit[j];
            CSCRecHit2D rechit = Make2DHits_->hitFromStripAndWire(sDetId, layer, w_hit, s_hit);
            bool isInFiducial = Make2DHits_->isHitInFiducial( layer, rechit );
            if ( isInFiducial ) {
              foundMatch = true;  
              hitsInLayer.push_back( rechit );
              hits_in_layer++;
            }
          }
        }
      }
    }

    if ( hits_in_layer < 1 && makePseudo2DHits ) { 
      // If missing wire hit in a layer, form pseudo 2-D hit from strip hit
      if ( debug ) std::cout << " Found gap in layer for wire hit collection " << std::endl;      
      if ( debug ) std::cout << "# strip hits in layer: "<< cscStripHit.size() << std::endl;

      for (unsigned i = 0; i != cscStripHit.size(); i++ ) {
        const CSCStripHit s_hit = cscStripHit[i];
        CSCRecHit2D rechit = Make2DHits_->hitFromStripOnly(sDetId, layer, s_hit);
        hitsInLayer.push_back( rechit );
        hits_in_layer++;
      }
    }

    // output vector of 2D rechits to collection
    if (hits_in_layer > 0) {
      oc.put( sDetId, hitsInLayer.begin(), hitsInLayer.end() );
      hitsInLayer.clear();
    }
    if ( debug ) std::cout << "# hits in layer: "<< hits_in_layer << std::endl;
    hits_in_layer = 0;
    layer_idx++;
    old_id = sDetId;
  }


/* Clear memory
 *  for ( CSCStripHitCollection::const_iterator sit = clean_soc.begin(); sit != clean_soc.end(); ++sit ){
 *    (*sit).clear();
 *  }
 */

}


/* getLayer
 *
 */
const CSCLayer* CSCRecHitBBuilder::getLayer( const CSCDetId& detId )  {
  if ( !geom_ ) throw cms::Exception("MissingGeometry") << "[CSCRecHitBBuilder::getLayer] Missing geometry" << std::endl;
  return geom_->layer(detId);
}

