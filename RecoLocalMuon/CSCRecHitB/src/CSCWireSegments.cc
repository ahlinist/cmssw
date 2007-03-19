/* This is CSCWireSegments
 *
 * \author: Dominique Fortin - UCR
 *
 */

#include <RecoLocalMuon/CSCRecHitB/src/CSCWireSegments.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCWireHitCollection.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCWireHit.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCRangeMapForRecHit.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>  

#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <vector>
#include <iostream>


CSCWireSegments::CSCWireSegments(const edm::ParameterSet& ps) : geom_(0) {
  
  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");
  minLayersApart         = ps.getUntrackedParameter<int>("CSCWireminLayersApart");
  proto_poca             = ps.getUntrackedParameter<double>("CSCWireMaxDistance");
  proto_deltaT           = ps.getUntrackedParameter<int>("CSCWireSegmentDeltaT"); 
  minWireHitsPerSegment  = ps.getUntrackedParameter<int>("CSCminWireHitsPerSegment");
  useHitsFromFits        = ps.getUntrackedParameter<bool>("CSCuseWireHitsFromFits");
  muonsPerChamberMax     = ps.getUntrackedParameter<int>("CSCSegmentPerChamberMax");  
}


CSCWireSegments::~CSCWireSegments(){

}


/* cleanWireHits
 *
 * This part sorts out the wire hits by chamber and call all the other modules 
 * which forms the wire segments, and removed the hits unused.
 *
 */
CSCWireHitCollection CSCWireSegments::cleanWireHits(const CSCWireHitCollection& raw_whit) {
  
  std::vector<CSCDetId> chambers;
  std::vector<CSCDetId>::const_iterator chIt;
  CSCWireHitCollection clean_whit_coll;

  if (raw_whit.size() < 3) return clean_whit_coll;
  
  // Sort hits by chambers 
  for ( CSCWireHitCollection::const_iterator it2 = raw_whit.begin(); it2 != raw_whit.end(); it2++ ) {
    bool insert = true;
    for ( chIt=chambers.begin(); chIt != chambers.end(); ++chIt ) {
      if (((*it2).cscDetId().chamber() == (*chIt).chamber()) &&
	  ((*it2).cscDetId().station() == (*chIt).station()) &&
	  ((*it2).cscDetId().ring()    == (*chIt).ring()) &&
	  ((*it2).cscDetId().endcap()  == (*chIt).endcap()))
	insert = false;
    }
    if ( insert ) chambers.push_back((*it2).cscDetId().chamberId());
  }
  
  if (debug) std::cout << "[CSCWireSegments::cleanWireHits] Number of chambers with hits is: " << chambers.end()-chambers.begin() << std::endl;
  
  // Now, create vector of hits for each chamber
  for ( chIt=chambers.begin(); chIt != chambers.end(); ++chIt ) {
    
    hitsInChamber.clear();
    
    ChamberHitContainer cscWireHit;
    
    CSCRangeMapForRecHit acc;
    CSCWireHitCollection::range range = raw_whit.get(acc.cscChamber(*chIt));
    
    for ( CSCWireHitCollection::const_iterator raw_whit = range.first; raw_whit != range.second; raw_whit++)
      cscWireHit.push_back(*raw_whit);    
    
    // Try to build segment
    findWireSegments(cscWireHit);
    
    // Order hits on segments per layer # and store to Collection
    for ( int layer = 1; layer < 7; layer++ ) {
      
      std::vector<CSCWireHit> hitsInLayer;
      bool found_hit_in_layer = false;
      CSCDetId id;
      
      // Loop over hits in proto segment
      for ( ChamberHitContainerCIt hi = hitsInChamber.begin(); hi != hitsInChamber.end();  ++hi ) {
	
	CSCWireHit h1 = *hi;
	int l1 = h1.cscDetId().layer();
	const CSCDetId id2 = h1.cscDetId();
	
	if ( l1 == layer ) {
	  found_hit_in_layer = true;
	  hitsInLayer.push_back( h1 );
	  id = id2;
	}
      }
      if (found_hit_in_layer) clean_whit_coll.put( id, hitsInLayer.begin(), hitsInLayer.end() );
    }   
  }
  return clean_whit_coll;
}


/* findWireSegments
 * 
 * This part builds the segments by first creating proto-segments from at least 3 hits
 * We intend to try all possible pairs of hits to start segment building. 'All possible' 
 * means each hit lies on different layers in the chamber.
 * BUT... once a hit has been assigned to a segment, we don't consider it again.
 *
 */
void CSCWireSegments::findWireSegments(const ChamberHitContainer& wirehit) {
  
  // IMPORTANT:  z = layer# -1, such that z(layer_1) = 0 
  
  ChamberHitContainerCIt ib = wirehit.begin();
  ChamberHitContainerCIt ie = wirehit.end();
  
  // Initialize flags that a given hit has been allocated to a segment
  for ( unsigned i = 0; i < 200; i++ ) usedHits[i] = 2;
  for ( unsigned i = 0; i < wirehit.size(); i++ ) usedHits[i] = 0; 
  
  
  // Allow to have at maximum muonsPerChamberMax muons tracks in a given chamber...
  for ( int pass = 0; pass < muonsPerChamberMax; pass++) {    
        
    // Now Loop over hits within the chamber to find 1st seed for segment building
    for ( ChamberHitContainerCIt i1 = ib; i1 < ie; ++i1 ) {
      if ( usedHits[i1-ib] > 1 ) continue;
      
      int layer1 = i1->cscDetId().layer();
      const CSCWireHit& h1 = *i1;
            
      // Loop over hits backward to find 2nd seed for segment building
      for ( ChamberHitContainerCIt i2 = ie-1; i2 > ib; --i2 ) {
	
	proto_segment.clear();
	if ( usedHits[i1-ib] > 1 ) continue;   // Hit has been used already
	if ( usedHits[i2-ib] > 1 ) continue;
	
	int layer2 = i2->cscDetId().layer();
	
	if ( (layer2 - layer1) < minLayersApart ) continue;
	
	bool segok = false;
	const CSCWireHit& h2 = *i2;					

        proto_slope = float( h2.wHitPos() - h1.wHitPos() ) / float( layer2 - layer1 ) ;
    
        // CAREFUL HERE:
        // Have to figure out what is intercept
        // Remember to use  z = layer# -1  such that  z = 0 for layer 1:  
    
        if ( layer1 == 1 ) {
          proto_intercept = float( h1.wHitPos() );
        } else {  
          proto_intercept = float( h1.wHitPos() ) - proto_slope * (layer1 - 1);
	}
	// Expected position for each layer is:
	for ( int j = 0; j < 6; j++) proto_y[j] = j * proto_slope + proto_intercept;
	proto_Chi2 = 0.;
	
	// Try adding hits to proto segment
	tryAddingHitsToSegment(wirehit, i1, i2); 
	
	// Check no. of hits on segment, and if enough flag them as used and store the segment
	segok = isSegmentGood(wirehit);
	if ( segok ) {
	  if ( proto_segment.empty() ) {
	    proto_segment.clear();
	  } else {
	    // Flag used hits
	    storeChamberHits();
	    if (wirehit.size() - proto_segment.size() < 3) return;
	    flagHitsAsUsed(wirehit);
	  }
	}
      } 
    } 
  }
}


/* Method tryAddingHitsToSegment
 *
 * Look at left over hits and try to add them to proto segment by looking how far they
 * are from the wire segment.  Here we don't really compute the DOCA, just the difference 
 * between the wgroup and wgroup expected from segment for that layer.
 *
 */
void CSCWireSegments::tryAddingHitsToSegment( const ChamberHitContainer& wirehit, 
                                              const ChamberHitContainerCIt i1, 
                                              const ChamberHitContainerCIt i2) {
  
  // Iterate over the layers with hits in the chamber
  // Skip layers containing the segment points on first passe, but then
  // in 2nd pass, test each hit on the layers to see if it is nearer the segment  
  
  ChamberHitContainerCIt ib = wirehit.begin();
  ChamberHitContainerCIt ie = wirehit.end();
  closeHits.clear();
  
  for ( ChamberHitContainerCIt i = ib; i != ie; ++i ) {
    
    if ( usedHits[i-ib] > 1) continue;   // Don't use hits already part of a segment.
    
    if (i == i1 || i == i2 ) continue;   // For first pass, don't try changing endpoints (seeds).
    
    int layer = i->cscDetId().layer();
    const CSCWireHit& h = *i;
      
    if ( isHitNearSegment( h ) ) {
      if ( hasHitOnLayer(layer) ) {
	closeHits.push_back(h);
      } 
    } else {
      increaseProtoSegment(h, layer);
    }
  } 

  fitSlope();
  if (closeHits.size() < 1 ) return;

  for ( ChamberHitContainerCIt i = closeHits.begin(); i != closeHits.end(); ++i ) {     
    int layer = i->cscDetId().layer();
    const CSCWireHit& h = *i;  
    compareProtoSegment(h, layer); 
  }
}


/* isHitNearSegment
 *
 * Just compare wire group # from CSCWireHit with expected one from proto segment
 */
bool CSCWireSegments::isHitNearSegment( const CSCWireHit& h ) const {
  
  // Again, remember that z=0 for layer 1 by default
  int idx = h.cscDetId().layer() - 1;
  float diff = float( h.wHitPos() ) - proto_y[idx];
  
  if ( diff >= -1.* proto_poca && diff <= proto_poca ) return true;
  
  return false;
}


/* Method addHit
 *
 * Test if can add hit to proto segment. If so, try to add it.
 *
 */
bool CSCWireSegments::addHit(const CSCWireHit& aHit, int layer) {
  
  // Return true if hit was added successfully and then parameters are updated.
  // Return false if there is already a hit on the same layer, or insert failed.
  
  bool ok = true;
  
  // Test that the wire hits occur close in time    
  if ( proto_segment.size() > 0) 
    if ( abs(proto_segment[0].tmax() - aHit.tmax()) > proto_deltaT ) ok = false;
  
  
  // Test that we are not trying to add the same hit again
  for ( ChamberHitContainer::const_iterator it = proto_segment.begin(); it != proto_segment.end(); it++ ) 
    if ( ( it->cscDetId().layer() == layer ) && ( aHit.wHitPos() == it->wHitPos() ) ) ok = false;
  
  if ( ok ) proto_segment.push_back(aHit);
  
  return ok;
}    


/* Method fitSlope
 *
 * Perform a simple Least Square Fit on proto segment to determine slope and intercept
 *
 */
void CSCWireSegments::fitSlope() {
  
  // Initialize parameters needed for Least Square fit:      
  float sz = 0.0; 
  float sy = 0.0; 
  float sz2 = 0.0; 
  float szy = 0.0; 
  int ns = proto_segment.size();
  
  for (ChamberHitContainer::const_iterator it = proto_segment.begin(); it != proto_segment.end(); it++ ) {
    
    // Again, by definition, we set z = layer# - 1;
    float z = float( it->cscDetId().layer() ) - 1.;
    float y = float( it->wHitPos() );
    sz  += z;
    sz2 += z*z;
    sy  += y;
    szy += z*y;
  }
  
  float denominator = (ns * sz2) - (sz * sz);
  
  if ( denominator != 0. ) {
    proto_intercept = ( (sy * sz2) - (sz * szy) ) / denominator;
    proto_slope     = ( (ns * szy) - (sy * sz) )  / denominator;         // Slope is in units of wire # / layer
  } else {
    proto_intercept = 999.;
    proto_slope = 999.;
  }
  // These are the expected position for each layer
  for ( int j = 0; j < 6; j++) proto_y[j] = j * proto_slope + proto_intercept;

  float chisq = 0.;
  
  // For now, don't use wire but wire group --> so error is just 1/sqrt(12)

  float sigma2 = 1. /12.;
  
  for (ChamberHitContainer::const_iterator ih = proto_segment.begin(); ih != proto_segment.end(); ++ih ) {
    // Again, I have set z(layer_1) = 0.
    int idx = ih->cscDetId().layer() -1; 
    float y = float( ih->wHitPos() );
    chisq += (y - proto_y[idx]) * (y - proto_y[idx]) / sigma2; 
  }
  proto_Chi2 = chisq;
}


/* hasHitOnLayer
 *
 * Just make sure hit to be added to layer comes from different layer than those in proto segment   
 */
bool CSCWireSegments::hasHitOnLayer(int layer) const {
  
  // Is there already a hit on this layer?
  for ( ChamberHitContainerCIt it = proto_segment.begin(); it != proto_segment.end(); it++ )
    if ( it->cscDetId().layer() == layer ) return true;
  
  return false;
}


/* Method compareProtoSegment
 *      
 * For hit coming from the same layer of an existing hit within the proto segment
 * test if achieve better chi^2 by using this hit than the other
 *
 */ 
void CSCWireSegments::compareProtoSegment(const CSCWireHit& h, int layer) {
  
  // Store old segment first
  float old_proto_Chi2                  = proto_Chi2;
  float old_proto_intercept             = proto_intercept;
  float old_proto_slope                 = proto_slope;
  ChamberHitContainer old_proto_segment = proto_segment;
  float old_proto_y[6];
  for (int j=0; j < 6; j++) old_proto_y[j] = proto_y[j]; 

  bool ok = replaceHit(h, layer);
  if (ok) fitSlope();
  
  if ( (proto_Chi2 < old_proto_Chi2) && (ok) ) return;
  
  proto_Chi2      = old_proto_Chi2;
  proto_intercept = old_proto_intercept;
  proto_slope     = old_proto_slope;
  proto_segment   = old_proto_segment;
  for (int j=0; j < 6; j++) proto_y[j] = old_proto_y[j];

}


/* Method replaceHit
 * 
 * Try adding the hit to existing segment, and remove old one existing in same layer
 *
 */
bool CSCWireSegments::replaceHit(const CSCWireHit& h, int layer) {
  
  // replace a hit from a layer
  ChamberHitContainer::iterator it;
  for ( it = proto_segment.begin(); it != proto_segment.end();) {
    if ( it->cscDetId().layer() == layer ) {
      it = proto_segment.erase(it);
    } else {
      ++it;
    }
  }
  return addHit(h, layer);
}


/* Method increaseProtoSegment
 *
 * For hit coming from different layer of an existing hit within the proto segment
 * see how far it falls from projected segment position and add if close enough
 *
 */     
void CSCWireSegments::increaseProtoSegment(const CSCWireHit& h, int layer) {
  
  // Store old segment first
  float old_proto_Chi2                  = proto_Chi2;
  float old_proto_intercept             = proto_intercept;
  float old_proto_slope                 = proto_slope;
  ChamberHitContainer old_proto_segment = proto_segment;
  float old_proto_y[6];
  for (int j=0; j < 6; j++) old_proto_y[j] = proto_y[j];
  bool ok = false;
  
  // Test that new hit fits closely to existing segment
  float y = float( h.wHitPos() );
  float diff = y - proto_y[layer-1];
  
  if ( diff >= -1. * proto_poca && diff <= proto_poca ) ok = addHit(h, layer);
  
  if ( ok ) return;
    
  proto_Chi2      = old_proto_Chi2;
  proto_intercept = old_proto_intercept;
  proto_slope     = old_proto_slope;
  proto_segment   = old_proto_segment;
  for (int j=0; j < 6; j++) proto_y[j] = old_proto_y[j];

}


/* Method isSegmentGood
 *
 * Look at how many wire hit we have in chamber
 * If the chamber has 20 hits or fewer, require at least 3 hits on segment
 * If the chamber has >20 hits require at least 4 hits
 *
 */
bool CSCWireSegments::isSegmentGood(const ChamberHitContainer& WireHitsInChamber) const {
  bool ok = false;
  unsigned int iadd = ( WireHitsInChamber.size() > 20 )? iadd = 1 : 0;  
  if (proto_segment.size() >= minWireHitsPerSegment+iadd) ok = true;
  return ok;
}


/* Method flagHitsAsUsed
 *
 * Flag hits which have entered segment building so we don't reuse them.
 * UsedHits = 0  --> unused
 *          = 1  --> near existing segment
 *          = 2  --> used
 */
void CSCWireSegments::flagHitsAsUsed(const ChamberHitContainer& WireHitsInChamber) {
  
  // Flag hits on segment as used
  ChamberHitContainerCIt ib = WireHitsInChamber.begin();
  
  // Loop over segment
  for ( ChamberHitContainerCIt hi = proto_segment.begin(); hi != proto_segment.end();  ++hi ) {
    
    //Loop over chamber hits
    for ( ChamberHitContainerCIt iu = WireHitsInChamber.begin(); iu != WireHitsInChamber.end(); ++iu ) {
      CSCWireHit h1 = *hi;
      CSCWireHit h2 = *iu;		
      if ( ( h1.wHitPos() == h2.wHitPos() ) &&	
	   ( h1.cscDetId().layer() == h2.cscDetId().layer() ) )	
	// Flag hit has used     
	usedHits[iu-ib] = 2;
    }
  }

  // Loop over hits near segment and remove
  for ( ChamberHitContainerCIt hi = closeHits.begin(); hi != closeHits.end();  ++hi ) {    
    //Loop over chamber hits
    for ( ChamberHitContainerCIt iu = WireHitsInChamber.begin(); iu != WireHitsInChamber.end(); ++iu ) {
      CSCWireHit h1 = *hi;
      CSCWireHit h2 = *iu;		
      if ( ( h1.wHitPos() == h2.wHitPos() ) &&	
	   ( h1.cscDetId().layer() == h2.cscDetId().layer() ) )	
	// Flag hit has used     
	usedHits[iu-ib] = 2;
    }
  }
}


/* Method storeChamberHits
 *
 * Add the wire hits from the successfull proto segment within a list CSCWireHits for this chamber
 *
 * Also add the hits which were far away from segments, that is flag as 0
 */
void CSCWireSegments::storeChamberHits() {
  // Loop over hits in proto segment
  for ( ChamberHitContainerCIt hi = proto_segment.begin(); hi != proto_segment.end();  ++hi ) {
    CSCWireHit h1 = *hi;
    if ( !useHitsFromFits ) {
      hitsInChamber.push_back( h1 );
    } else {
      int this_layer = (*hi).cscDetId().layer();
      float this_wgroup = proto_y[this_layer-1];
      CSCDetId this_id = (*hi).cscDetId();
      int this_tmax = (*hi).tmax();  
      CSCWireHit::ChannelContainer wgroups = (*hi).wgroups();
      CSCWireHit this_wirehit( this_id, this_wgroup, wgroups, this_tmax );
      hitsInChamber.push_back( this_wirehit );
    }
  }
}


/*
 *
 */
const CSCLayer* CSCWireSegments::getLayer( const CSCDetId& detId )  {
  if ( !geom_ ) throw cms::Exception("MissingGeometry") << "[CSCWireSegments::getLayer] Missing geometry" << std::endl;
  return geom_->layer(detId);
}


