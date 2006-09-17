/* This is CSCHitFromWireOnly
 *
 * Finds wiregroup with hits, and fill in CSCWireHitCollection
 * which includes only DetId and wiregroup #
 *
 * \author  Dominique Fortin - UCR
 *
 */

#include <RecoLocalMuon/CSCRecHitB/src/CSCHitFromWireOnly.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>

#include <DataFormats/CSCRecHit/interface/CSCWireHit.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <iostream>

CSCHitFromWireOnly::CSCHitFromWireOnly( const edm::ParameterSet& ps ) {
  
  debug      = ps.getUntrackedParameter<bool>("CSCDebug");
  deltaT     = ps.getUntrackedParameter<int>("CSCWireClusterDeltaT");
  clusterSize= ps.getUntrackedParameter<int>("CSCWireClusterMaxSize");
  
}


CSCHitFromWireOnly::~CSCHitFromWireOnly(){}


/* runWire
 *
 */
std::vector<CSCWireHit> CSCHitFromWireOnly::runWire( const CSCDetId& id, const CSCLayer* layer, const CSCWireDigiCollection::Range& rwired ) {
  
  std::vector<CSCWireHit> hitsInLayer;
  
  layer_ = layer;
  layergeom_ = layer->geometry();
  bool any_digis = true;
  int n_wgroup = 0;
  
  for ( CSCWireDigiCollection::const_iterator it = rwired.first; it != rwired.second; ++it ) {
    
    const CSCWireDigi wdigi = *it;
    
    if ( any_digis ) {
      any_digis = false;
      n_wgroup = 1;
      if ( rwired.second - rwired.first == 1) {
	makeWireCluster( wdigi );
	float whit_pos = findWireHitPosition();
	CSCWireHit whit(id, whit_pos, theTime); 
	hitsInLayer.push_back( whit );
      } else { 
	makeWireCluster( wdigi );
      }
    } else {
      if ( !addToCluster( wdigi ) ) {
	// Make Wire Hit from cluster, delete old cluster and start new one
	if (n_wgroup > 2) std::cout << "Found wire cluster of " << n_wgroup << " wire groups " << std::endl;
	float whit_pos = findWireHitPosition();
	CSCWireHit whit(id, whit_pos, theTime);
	hitsInLayer.push_back( whit );	
	makeWireCluster( wdigi );
	n_wgroup = 1;
      } else {
	n_wgroup++;
      }
    }
    // Don't forget to fill last wire hit !!!
    if ( it - rwired.first == 1) {           
      float whit_pos = findWireHitPosition();
      CSCWireHit whit(id, whit_pos, theTime);
      hitsInLayer.push_back( whit );
    }
  }
  return hitsInLayer;
}


/* makeWireCluster
 *
 */
void CSCHitFromWireOnly::makeWireCluster(const CSCWireDigi & digi) {
  wire_cluster.clear();
  theLastChannel  = digi.getWireGroup();
  theTime         = digi.getTimeBin();
  wire_cluster.push_back( digi );
}


/* addToCluster
 *
 */
bool CSCHitFromWireOnly::addToCluster(const CSCWireDigi & digi) {
  bool value = false;
  int iwg = digi.getWireGroup();
  if ( (iwg == theLastChannel+1) && (abs(digi.getTimeBin()-theTime)<= deltaT) ) {
    value = true;
    theLastChannel = iwg;
    wire_cluster.push_back( digi );
  }
  return value;
}


/* findWireHitPosition
 *
 * This position is expressed in terms of wire #... is a float since it may be a fraction.
 */
float CSCHitFromWireOnly::findWireHitPosition() {
  
  // Again use center of mass to determine position of wire hit
  // To do so, need to know wire spacing and # of wires
  
  float wire_pos = -999.;
  float weighting = 0.0;
  float y = 0.0;
  
  for ( unsigned i = 0; i < wire_cluster.size(); i++ ) {
    CSCWireDigi wdigi = wire_cluster[i];
    int channel = wdigi.getWireGroup();
    int nwires = layergeom_->numberOfWiresPerGroup(channel); 
    float w_channel = float ( nwires );
    y += w_channel * channel;
    weighting += w_channel;
  }       
  float wiregpos = y / weighting;

  int wgroup = int( wiregpos );        // This is the corresponding wiregroup
  float woffset = wiregpos - wgroup ;
 
  float theFirstWire;
  float theLastWire;

  if ( woffset == 0 ) {
    wire_pos = layergeom_->middleWireOfGroup(wgroup);

  } else {
    float middleWire = layergeom_->middleWireOfGroup(1);   
    int nwires       = layergeom_->numberOfWiresPerGroup(1);
    theFirstWire     = ( middleWire - nwires/2. );
    theLastWire      = ( middleWire + nwires/2. );
    if ( woffset < 0.5 ) {
      wire_pos = 2. * (0.5 - woffset) * middleWire + 2. *woffset * theFirstWire;  // Keep proper normalization !!!
    } else {
      wire_pos = 2. * (0.5 - woffset) * theLastWire + 2. * woffset * middleWire;
    } 
  }   
  return wire_pos;
}

