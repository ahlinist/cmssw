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
  int nWireGroup = layergeom_->numberOfWireGroups();
  
  if ( wgroup < 1 ) {
    float middleWire = layergeom_->middleWireOfGroup(1);   
    int nwires = layergeom_->numberOfWiresPerGroup(1);
    int theFirstWire = int( middleWire - nwires/2. + 0.5 );
    int theLastWire  = int( middleWire + nwires/2. );
    float center_wgroup = (theFirstWire + theLastWire)/2.;
    if ( woffset > 0.5 ) {
      wire_pos = (0.5 - woffset) * theFirstWire + woffset * center_wgroup;
    } else {
      wire_pos = theFirstWire;
    }
    
  } else if (woffset == 0.) {
    float middleWire = layergeom_->middleWireOfGroup(wgroup);
    int nwires = layergeom_->numberOfWiresPerGroup(wgroup);
    int theFirstWire = int( middleWire - nwires/2. + 0.5 );
    int theLastWire  = int( middleWire + nwires/2. );
    wire_pos = (theFirstWire + theLastWire)/2.;
    
  } else if ( wgroup > nWireGroup ) {
    float middleWire = layergeom_->middleWireOfGroup(nWireGroup);
    int nwires = layergeom_->numberOfWiresPerGroup(nWireGroup);
    int theFirstWire = int( middleWire - nwires/2. + 0.5 );
    int theLastWire  = int( middleWire + nwires/2. );
    float center_wgroup = (theFirstWire + theLastWire)/2.;
    if ( woffset < 0.5 ) {
      wire_pos = (0.5 - woffset) * center_wgroup + woffset * theLastWire ;
    } else {
      wire_pos = theLastWire;
    }
    
  } else {
    float middleWire_0 = layergeom_->middleWireOfGroup(wgroup);
    int nwires_0 = layergeom_->numberOfWiresPerGroup(wgroup);
    int theFirstWire_0 = int( middleWire_0 - nwires_0/2. + 0.5 );
    int theLastWire_0  = int( middleWire_0 + nwires_0/2. );
    float center_wgroup_0 = (theFirstWire_0 + theLastWire_0)/2.;   // That's the center of the wire group0
    
    float middleWire_1 = layergeom_->middleWireOfGroup(wgroup+1);
    int nwires_1 = layergeom_->numberOfWiresPerGroup(wgroup+1);
    int theFirstWire_1 = int( middleWire_1 - nwires_1/2. + 0.5 );
    int theLastWire_1  = int( middleWire_1 + nwires_1/2. );
    float center_wgroup_1 = (theFirstWire_1 + theLastWire_1)/2.;   // That's the center of the wire group1
    
    wire_pos = (1.0 - woffset) * center_wgroup_0 + woffset * center_wgroup_1;  // scale accordingly to find center of mass
  }   
  return wire_pos;
}

