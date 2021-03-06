/* This is CSCStripSegments
 *
 * \author: Dominique Fortin - UCR
 *
 */

#include <RecoLocalMuon/CSCRecHitB/src/CSCStripSegments.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCStripHit.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCStripHitCollection.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCRangeMapForRecHit.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>  
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>
  
#include <vector>
#include <iostream>

CSCStripSegments::CSCStripSegments(const edm::ParameterSet& ps) : geom_(0) {
  
  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");
  minLayersApart         = ps.getUntrackedParameter<int>("CSCStripminLayersApart");
  proto_poca             = ps.getUntrackedParameter<double>("CSCStripMaxDistance");
  isFarFromSegment       = ps.getUntrackedParameter<double>("CSCStripCloseToSegment");
  proto_deltaT           = ps.getUntrackedParameter<int>("CSCStripSegmentDeltaT"); 
  minStripHitsPerSegment = ps.getUntrackedParameter<int>("CSCminStripHitsPerSegment");
  useHitsFromFits        = ps.getUntrackedParameter<bool>("CSCuseStripHitsFromFits");
  muonsPerChamberMax     = ps.getUntrackedParameter<int>("CSCSegmentPerChamberMax"); 
  storeLeftOvers         = ps.getUntrackedParameter<bool>("CSCuseLeftOverStripHits");
}


CSCStripSegments::~CSCStripSegments(){}


/* cleanStripHits
 *
 * This part sorts out the strip hits by chamber and call all the other modules 
 * which forms the strip segments, and removed the hits unused.
 *
 */
CSCStripHitCollection CSCStripSegments::cleanStripHits(const CSCStripHitCollection& raw_shit) {

  std::vector<CSCDetId> chambers;
  std::vector<CSCDetId>::const_iterator chIt;
  CSCStripHitCollection clean_shit_coll;
  
  // Sort hits by chambers 
  for ( CSCStripHitCollection::const_iterator it2 = raw_shit.begin(); it2 != raw_shit.end(); it2++ ) {
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

  if (debug) std::cout <<"[CSCStripSegments::cleanStripHits] Number of chambers with hits is " << chambers.end() - chambers.begin() << std::endl;
  
  // Now, create vector of hits for each chamber
  for ( chIt=chambers.begin(); chIt != chambers.end(); ++chIt ) {
    
    hitsInChamber.clear();
    
    ChamberHitContainer cscStripHit;
    
    CSCRangeMapForRecHit acc;
    CSCStripHitCollection::range range = raw_shit.get(acc.cscChamber(*chIt));
    
    for ( CSCStripHitCollection::const_iterator raw_shit = range.first; raw_shit != range.second; raw_shit++)
      cscStripHit.push_back(*raw_shit);
    
    // Try to build segment
    findStripSegments(cscStripHit);
    
    // Order hits on segments per layer # and store to Collection
    for ( int layer = 1; layer < 7; layer++ ) {
      
      std::vector<CSCStripHit> hitsInLayer;
      bool found_hit_in_layer = false;
      CSCDetId id;
      
      // Loop over hits in proto segment
      for ( ChamberHitContainerCIt hi = hitsInChamber.begin(); hi != hitsInChamber.end();  ++hi ) {
	
	CSCStripHit h1 = *hi;
	int l1 = h1.cscDetId().layer();
	const CSCDetId id2 = h1.cscDetId();
	
	if ( l1 == layer ) {
	  found_hit_in_layer = true;
	  hitsInLayer.push_back( h1 );
	  id = id2;
	}
      }
      if (found_hit_in_layer) clean_shit_coll.put( id, hitsInLayer.begin(), hitsInLayer.end() );
    }   
  }
  return clean_shit_coll;
}


/* findStripSegments
 * 
 * This part builds the segments by first creating proto-segments from at least 3 hits
 * We intend to try all possible pairs of hits to start segment building. 'All possible' 
 * means each hit lies on different layers in the chamber.
 * BUT... once a hit has been assigned to a segment, we don't consider it again.
 *
 */
void CSCStripSegments::findStripSegments(const ChamberHitContainer& striphit) {
  
  // IMPORTANT:  z = layer# -1, such that z(layer_1) = 0 
  
  ChamberHitContainerCIt ib = striphit.begin();
  ChamberHitContainerCIt ie = striphit.end();
  
  // Initialize flags that a given hit has been allocated to a segment
  for ( unsigned i = 0; i < 200; i++ ) usedHits[i] = 2;
  for ( unsigned i = 0; i < striphit.size(); i++ ) usedHits[i] = 0; 
  
  if ( ie - ib <  minStripHitsPerSegment ) { 
    if (storeLeftOvers && ie - ib > 0 ) storeLeftOverHits( striphit );   // not enough hits to build segment, but store hits
    return;
  }
  
  // Allow to have at maximum muonsPerChamberMax muons tracks in a given chamber...
  for ( int pass = 0; pass < muonsPerChamberMax; pass++) {    
    
    
    // Now Loop over hits within the chamber to find 1st seed for segment building
    for ( ChamberHitContainerCIt i1 = ib; i1 < ie; ++i1 ) {
      if ( usedHits[i1-ib] > 2 ) continue;
      
      int layer1 = i1->cscDetId().layer();
      const CSCStripHit& h1 = *i1;
      
      
      // Loop over hits backward to find 2nd seed for segment building
      for ( ChamberHitContainerCIt i2 = ie-1; i2 > ib; --i2 ) {
	
	proto_segment.clear();
	if ( usedHits[i1-ib] > 2 ) continue;   // Hit has been used already
	if ( usedHits[i2-ib] > 2 ) continue;
	
	int layer2 = i2->cscDetId().layer();
	
	if ( (layer2 - layer1) < minLayersApart ) continue;
	
	bool segok = false;
	const CSCStripHit& h2 = *i2;					
	
	if ( !addHit(h1, layer1) ) continue;
	if ( !addHit(h2, layer2) ) continue;
	
	// Try adding hits to proto segment
	tryAddingHitsToSegment(striphit, i1, i2); 
	
	// Check no. of hits on segment, and if enough flag them as used and store the segment
	segok = isSegmentGood(striphit);
	if ( segok ) {
	  if ( proto_segment.empty() ) {
	    proto_segment.clear();
	  } else {
	    // Flag used hits
	    flagHitsAsUsed(striphit);
	    storeChamberHits();
	    proto_segment.clear();
	  }
	}
      } 
    } 
  }
  if (storeLeftOvers) storeLeftOverHits( striphit );   // Also save hits which are far from existing segments
}


/* Method addHit
 *
 * Test if can add hit to proto segment. If so, try to add it.
 *
 */
bool CSCStripSegments::addHit(const CSCStripHit& aHit, int layer) {
  
  // Return true if hit was added successfully and then parameters are updated.
  // Return false if there is already a hit on the same layer, or insert failed.
  
  bool ok = true;
  
  // Test that the strip hits occur close in time    
  if ( proto_segment.size() > 0) 
    if ( abs(proto_segment[0].tmax() - aHit.tmax()) > proto_deltaT ) ok = false;
  
  
  // Test that we are not trying to add the same hit again
  for ( ChamberHitContainer::const_iterator it = proto_segment.begin(); it != proto_segment.end(); it++ ) 
    if ( ( it->cscDetId().layer() == layer ) && ( aHit.sHitPos() == it->sHitPos() ) ) ok = false;
  
  if ( ok ) {
    proto_segment.push_back(aHit);
    updateParameters();
  }
  return ok;
}    


/* Method updateParameters
 *      
 * If have only 2 hits, form a seed segment by computing slope between 2 points and intercept
 * If have more than 2 hits, perform Least Square Fit
 *
 * N.B. Perform calculation in "y" space and than translate the expected values in strip position
 * for each layer.  Hence the proto_intercept and proto_slope are in "y" space an no strip #
 */   
void CSCStripSegments::updateParameters() {
  
  int nh = proto_segment.size();
  
  // First hit added to a segment must always fail here
  if ( nh < 2 ) return;
  
  if ( nh == 2 ) {
    // Once we have two hits we can compute a straight line between the 2 points.
    
    ChamberHitContainer::const_iterator ih = proto_segment.begin();
    int il1 = (*ih).cscDetId().layer();
    const CSCStripHit& h1 = (*ih);
    float offset1 = getStripOffset( h1 );
    ++ih;
    int il2 = (*ih).cscDetId().layer();
    const CSCStripHit& h2 = (*ih);   
    float offset2 = getStripOffset( h2 );
    
    // If on same layer, at this point reject
    if ( il1 == il2 ) return;
    
    // Take into account the offset between strip # of different layers   
    float y1 = float( h1.sHitPos()) + offset1;
    float y2 = float( h2.sHitPos()) + offset2;
    proto_slope = (y2 - y1) / float( il2 - il1 );        
    
    // Have to figure out what is intercept in "y" space
    // For this purpose, use  z = layer# -1  such that z = 0 for layer 1:
    proto_intercept = y1 - proto_slope * (il1-1);
    
    
    // I know what the offset and layer # for y1.  I can work my way backward and find the
    // offset at layer 1;
    
    float sign = 1.;
    for ( int k = il1; k > 0; k--) {
      int n = il1 - k;
      sign = pow(-1., n);
    }
    float offset0 = offset1 * sign;
    
    
    // These are the expected strip position for each layer
    for ( int j = 0; j < 6; j++) {
      int n = j%2;
      sign = pow(-1., n);
      proto_y[j] = j * proto_slope + proto_intercept + offset0 * sign;
    }
    proto_Chi2 = 0.;
    return;
  }
  
  // When we have more than two hits then we can perform Least Square Fit on points
  fitSlope();
  fillChiSquared();
}


/* fitSlope
 *
 * Perform a simple Least Square Fit on proto segment to determine slope and intercept
 * Perform the fit in "y" space and than translate the expected values in strip position 
 * for each layer.
 */
void CSCStripSegments::fitSlope() {
  
  // Initialize parameters needed for Least Square fit:      
  float sz = 0.0; 
  float sy = 0.0; 
  float sz2 = 0.0; 
  float szy = 0.0; 
  int ns = proto_segment.size();
  int last_layer = 0;
  float offset1=0.;
  
  for (ChamberHitContainer::const_iterator it = proto_segment.begin(); it != proto_segment.end(); it++ ) {
    
    // Again, by definition, we set z = layer# - 1;
    last_layer = it->cscDetId().layer();
    float z = float( last_layer ) -1.;
    const CSCStripHit& h1 = (*it);
    offset1 = getStripOffset( h1 );
    // Take into account the offset between strip # of different layers
    float y = float( h1.sHitPos()) + offset1;
    
    sz  += z;
    sz2 += z*z;
    sy  += y;
    szy += z*y;
  }
  
  float denominator = (ns * sz2) - (sz * sz);
  
  if ( denominator != 0. ) {
    proto_intercept = ( (sy * sz2) - (sz * szy) ) / denominator;
    proto_slope     = ( (ns * szy) - (sy * sz) )  / denominator;         // Slope is in units of strip # / layer
  } else {
    proto_intercept = 999.;
    proto_slope = 999.;
  }
  
  
  // I know what the offset and layer # was for the last entry.  I can work my way backward and find the 
  // offset at layer 1;    
  float sign = 1.;
  for ( int k = last_layer; k > 0; k--) {
    int n = last_layer - k;
    sign = pow(-1., n);        
  }
  float offset0 = offset1 * sign;
  
  // These are the expected strip position for each layer
  for ( int j = 0; j < 6; j++) {
    int n = j%2;
    sign = pow(-1., n);
    proto_y[j] = j * proto_slope + proto_intercept + offset0 * sign;
  }
}


/* Method fillChiSquared
 *
 * Determine Chi^2 for the proto strip segment
 *
 */
void CSCStripSegments::fillChiSquared() {
  
  float chisq = 0.;
  
  // For uniform distribution, the error^2 is 1/12 --> here take 1/2 width of strip^2 / 12:
  
  float sigma2 = 1. /12.;
  
  for (ChamberHitContainer::const_iterator ih = proto_segment.begin(); ih != proto_segment.end(); ++ih ) {
    // Again, I have set z(layer_1) = 0.
    int idx = ih->cscDetId().layer() -1; 
    float y = float( ih->sHitPos() );
    chisq += (y - proto_y[idx]) * (y - proto_y[idx]) / sigma2; 
  }
  proto_Chi2 = chisq;
}


/* Method tryAddingHitsToSegment
 *
 * Look at left over hits and try to add them to proto segment by looking how far they
 * are from the strip segment.  Here we don't really compute the DOCA, just the difference 
 * between the wgroup and wgroup expected from segment for that layer.
 *
 */
void CSCStripSegments::tryAddingHitsToSegment( const ChamberHitContainer& striphit, 
					       const ChamberHitContainerCIt i1, 
					       const ChamberHitContainerCIt i2) {
  
  // Iterate over the layers with hits in the chamber
  // Skip the layers containing the segment endpoints on first 2 passes, but then
  // try hits on layer containing the segment starting points on 2nd and/or 3rd pass 
  // if segment has >2 hits
  // Test each hit on the other layers to see if it is near the segment
  // If it is, see whether there is already a hit on the segment from the same layer
  //    - if so, and there are more than 2 hits on the segment, copy the segment,
  //      replace the old hit with the new hit. If the new segment chi2 is better
  //      then replace the original segment with the new one (by swap)
  //    - if not, copy the segment, add the hit if it's within a certain range. 
  // Finally, on 5th pass, flag hits which are near the segment, but not part of it.
  
  
  ChamberHitContainerCIt ib = striphit.begin();
  ChamberHitContainerCIt ie = striphit.end();
  
  for ( int pass = 0; pass < 5; pass++) {
    
    for ( ChamberHitContainerCIt i = ib; i != ie; ++i ) {
      
      if ( usedHits[i-ib] > 2) continue;   // Don't use hits already part of a segment.
      
      if (pass < 2) if (i == i1 || i == i2 ) continue;  // For first 2 pass, don't try changing endpoints (seeds).
      
      int layer = i->cscDetId().layer();
      const CSCStripHit& h = *i;
      
      
      // This is the end of the loop
      if ( pass > 3 ) {
	// flag hits near segment if segment > 2 hits
	if ( proto_segment.size() > 2 ) flagHitNearSegment( h, i, ib );
	break;
      }
      
      
      if ( isHitNearSegment( h ) ) {
	if ( hasHitOnLayer(layer) ) {
	  // If segment > 2 hits, try changing endpoints
	  if ( proto_segment.size() > 2) {
	    compareProtoSegment(h, layer); 
	  } 
	} else {
	  increaseProtoSegment(h, layer);
	}
      } 
    }
  } 
}


/* isHitNearSegment
 *
 * Just compare strip group # from CSCStripHit with expected one from proto segment
 */
bool CSCStripSegments::isHitNearSegment( const CSCStripHit& h ) const {
  
  // Again, remember that z=0 for layer 1 by default
  int idx = h.cscDetId().layer() - 1;
  float diff = float( h.sHitPos() ) - proto_y[idx];
  
  if ( diff >= -1.* proto_poca && diff <= proto_poca ) return true;
  
  return false;
}


/* flagHitNearSegment
 *
 * Just compare strip hit position # with expected one from built proto segment
 * Flag hit as near segment if managed to build full segment, and also mark those far off.
 * This may be noise or a deposit from a muon which crossed path with existing segment.
 */
void CSCStripSegments::flagHitNearSegment( const CSCStripHit& h, 
					   const ChamberHitContainerCIt id1,
					   const ChamberHitContainerCIt id2) {
  
  // Again, remember that z=0 for layer 1 by default
  int idx = h.cscDetId().layer() - 1;
  float diff = float( h.sHitPos() ) - proto_y[idx];
  
  if ( diff >= -1.* proto_poca && diff <= 1. * proto_poca ) {
      usedHits[id1-id2] = 1;  // Mark those as close by
  } else if ( diff >= -1 * isFarFromSegment && diff <= isFarFromSegment ) {
      usedHits[id1-id2] = 2;  // Mark those as far off..
  }
}  


/* hasHitOnLayer
 *
 * Just make sure hit to be added to layer comes from different layer than those in proto segment   
 */
bool CSCStripSegments::hasHitOnLayer(int layer) const {
  
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
void CSCStripSegments::compareProtoSegment(const CSCStripHit& h, int layer) {
  
  // Store old segment first
  float old_proto_Chi2                  = proto_Chi2;
  float old_proto_intercept             = proto_intercept;
  float old_proto_slope                 = proto_slope;
  ChamberHitContainer old_proto_segment = proto_segment;
  float old_proto_y[6];
  for (int j=0; j < 6; j++) old_proto_y[j] = proto_y[j]; 

  bool ok = replaceHit(h, layer);
  
  if ( (proto_Chi2 < old_proto_Chi2) && (ok) ) {
    return;
  } else {
    proto_Chi2      = old_proto_Chi2;
    proto_intercept = old_proto_intercept;
    proto_slope     = old_proto_slope;
    proto_segment   = old_proto_segment;
    for (int j=0; j < 6; j++) proto_y[j] = old_proto_y[j]; 
  }
}


/* Method replaceHit
 * 
 * Try adding the hit to existing segment, and remove old one existing in same layer
 *
 */
bool CSCStripSegments::replaceHit(const CSCStripHit& h, int layer) {
  
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
 */     
void CSCStripSegments::increaseProtoSegment(const CSCStripHit& h, int layer) {
  
  // Store old segment first
  float old_proto_Chi2                  = proto_Chi2;
  float old_proto_intercept             = proto_intercept;
  float old_proto_slope                 = proto_slope;
  ChamberHitContainer old_proto_segment = proto_segment;
  float old_proto_y[6];
  for (int j=0; j < 6; j++) old_proto_y[j] = proto_y[j]; 
  
  bool ok = false;
  
  // Test that new hit fits closely to existing segment
  float y = float( h.sHitPos() );
  float diff = y - proto_y[layer-1];
  
  if ( diff >= -1. * proto_poca && diff <= proto_poca ) ok = addHit(h, layer);
  
  if ( ok ) {
    return;    
  } else {
    proto_Chi2      = old_proto_Chi2;
    proto_intercept = old_proto_intercept;
    proto_slope     = old_proto_slope;
    proto_segment   = old_proto_segment;
    for (int j=0; j < 6; j++) proto_y[j] = old_proto_y[j]; 
  }
}


/* Method isSegmentGood
 *
 * Look at how many strip hit we have in chamber
 * If the chamber has 20 hits or fewer, require at least 3 hits on segment
 * If the chamber has >20 hits require at least 4 hits
 */
bool CSCStripSegments::isSegmentGood(const ChamberHitContainer& StripHitsInChamber) const {
  bool ok = false;
  unsigned int iadd = ( StripHitsInChamber.size() > 20 )? iadd = 1 : 0;  
  if (proto_segment.size() >= minStripHitsPerSegment+iadd) ok = true;
  return ok;
}


/* Method flagHitsAsUsed
 *
 * Flag hits which have entered segment building so we don't reuse them.
 * UsedHits = 0  --> unused
 *          = 1  --> near existing segment
 *          = 2  --> very far from existing segment
 *          = 3  --> used
 */
void CSCStripSegments::flagHitsAsUsed(const ChamberHitContainer& StripHitsInChamber) {
  
  // Flag hits on segment as used
  ChamberHitContainerCIt ib = StripHitsInChamber.begin();
  
  // Loop over segment
  for ( ChamberHitContainerCIt hi = proto_segment.begin(); hi != proto_segment.end();  ++hi ) {
    
    //Loop over chamber hits
    for ( ChamberHitContainerCIt iu = StripHitsInChamber.begin(); iu != StripHitsInChamber.end(); ++iu ) {
      CSCStripHit h1 = *hi;
      CSCStripHit h2 = *iu;		
      if ( ( h1.sHitPos() == h2.sHitPos() ) &&	
	   ( h1.cscDetId().layer() == h2.cscDetId().layer() ) )
	
	// Flag hit has used     
	usedHits[iu-ib] = 3;
    }
  }
}


/* Method storeChamberHits
 *
 * Add the strip hits from the successull proto segment within a list CSCStripHits for this chamber
 *
 * Also add the hits which were near from segments, that is flag as 0
 */
void CSCStripSegments::storeChamberHits() {
  // Loop over hits in proto segment
  for ( ChamberHitContainerCIt hi = proto_segment.begin(); hi != proto_segment.end();  ++hi ) {
    CSCStripHit h1 = *hi;
    
    if ( !useHitsFromFits ) {
      hitsInChamber.push_back( h1 );
    } else {
      int this_layer = (*hi).cscDetId().layer();
      float this_strippos = proto_y[this_layer-1];
      CSCDetId this_id = (*hi).cscDetId();
      int this_tmax = (*hi).tmax();  
      CSCStripHit::ChannelContainer strips = (*hi).strips();
      std::vector<float> adcs = (*hi).s_adc();

      CSCStripHit this_striphit( this_id, this_strippos, this_tmax, strips, adcs );
      hitsInChamber.push_back( this_striphit );
    }
  }
}


/* storeLeftOverHits
 *
 * Loop over leftover hits:  if they are close from existing segments store this hit
 * Also store them if they have been unused.
 * Otherwise assume that they come from delta rays and ignore
 */
void CSCStripSegments::storeLeftOverHits(const ChamberHitContainer& striphit) {
  
  ChamberHitContainerCIt ib = striphit.begin();
  ChamberHitContainerCIt ie = striphit.end();
  
  for ( ChamberHitContainerCIt i = ib; i != ie; ++i ) {
    const CSCStripHit& h = *i;        
    if ( usedHits[i-ib] < 2 ) hitsInChamber.push_back( h );
  }
} 


/* setGeometry
 *
 */
void CSCStripSegments::setGeometry( const CSCGeometry* geom ) {
  geom_ = geom;
}


/* getLayer
 *
 */
const CSCLayer* CSCStripSegments::getLayer( const CSCDetId& detId ) {
  if ( !geom_ ) throw cms::Exception("MissingGeometry") << "[CSCStripSegments::getLayer] Missing geometry" << std::endl;
  return geom_->layer(detId);
}


/* getStripOffset
 *
 */
float CSCStripSegments::getStripOffset( const CSCStripHit& aHit ) {
  
  const CSCDetId id1 = aHit.cscDetId();
  const CSCLayer* layer_ = getLayer( id1 );
  const CSCLayerGeometry* layergeom_ = layer_->geometry();    
  float offset = layergeom_->stripOffset() ;
  
  return offset;
}
