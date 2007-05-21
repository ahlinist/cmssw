/* This is CSCHitFromWireOnly
 *
 * Finds wiregroup with hits, and fill in CSCWireHitCollection
 * which includes only DetId and wiregroup #
 *
 * \author  Dominique Fortin - UCR
 *
 */

#include <RecoLocalMuon/CSCRecHitB/src/CSCHitFromWireOnly.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCWireHit.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>

#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <iostream>

CSCHitFromWireOnly::CSCHitFromWireOnly( const edm::ParameterSet& ps ) {
  
  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");
  deltaT                 = ps.getUntrackedParameter<int>("CSCWireClusterDeltaT");
  clusterSize            = ps.getUntrackedParameter<int>("CSCWireClusterMaxSize");
  useCleanWireCollection = ps.getUntrackedParameter<bool>("CSCuseCleanWireCollection");  
}


CSCHitFromWireOnly::~CSCHitFromWireOnly(){}


/* runWire
 *
 */
std::vector<CSCWireHit> CSCHitFromWireOnly::runWire( const CSCDetId& id, const CSCLayer* layer, const CSCWireDigiCollection::Range& rwired, const CSCALCTDigiCollection* alcts ) {
  
  std::vector<CSCWireHit> hitsInLayer;

  std::vector<int> alctWgroup;
    
  layer_ = layer;
  layergeom_ = layer->geometry();
  bool any_digis = true;
  int n_wgroup = 0;

  // Get wire groups which are part of the ALCT
  if ( useCleanWireCollection ) alctWgroup = getALCTWgroup( id, alcts );

  // Loop over wire digi collection
  for ( CSCWireDigiCollection::const_iterator it = rwired.first; it != rwired.second; ++it ) {
    
    const CSCWireDigi wdigi = *it;

    // If useCleanWireCollection == true, test if wire digi fits ALCT wire condition
    if ( useCleanWireCollection ) {
      if ( !foundALCTMatch( wdigi, alctWgroup ) ) continue;
    }

    if ( any_digis ) {
      any_digis = false;
      makeWireCluster( wdigi );
      n_wgroup = 1;
    } else {
      if ( !addToCluster( wdigi ) ) {
	// Make Wire Hit from cluster, delete old cluster and start new one
	float whit_pos = findWireHitPosition();
	CSCWireHit whit(id, whit_pos, wire_in_cluster, theTime);
	hitsInLayer.push_back( whit );	
	makeWireCluster( wdigi );
	n_wgroup = 1;
      } else {
	n_wgroup++;
      }
    }
    // Don't forget to fill last wire hit !!!
    if ( rwired.second - it == 1) {           
      float whit_pos = findWireHitPosition();
      CSCWireHit whit(id, whit_pos, wire_in_cluster, theTime);
      hitsInLayer.push_back( whit );
      n_wgroup++;
    }
  }

  int nHits = hitsInLayer.size();

  if (nHits < 2) return hitsInLayer;

  // Loop over the wire hits to ensure don't have twice same wire hit
  // This is a problem in MTCC data

  std::vector<CSCWireHit> whits;

  bool isDuplicate = false;
  for ( int i = 0; i < nHits; ++i ) {
    isDuplicate = false;
    float w1 = hitsInLayer[i].wHitPos();

    for ( int j = 0; j < nHits; ++j ) {
      float w2 = hitsInLayer[j].wHitPos();	
      if ( w1 == w2 && i > j) isDuplicate = true;
    }

    if ( !isDuplicate ) whits.push_back(hitsInLayer[i]);
  }

  return whits;
}


/* makeWireCluster
 *
 */
void CSCHitFromWireOnly::makeWireCluster(const CSCWireDigi & digi) {
  wire_cluster.clear();
  wire_in_cluster.clear();
  theLastChannel  = digi.getWireGroup();
  theTime         = digi.getTimeBin();
  wire_cluster.push_back( digi );
}


/* addToCluster
 *
 */
bool CSCHitFromWireOnly::addToCluster(const CSCWireDigi & digi) {

  int iwg = digi.getWireGroup();

  if ( iwg == theLastChannel ) return true;  // Same wire group but different tbin -> ignore

  if ( (iwg == theLastChannel+1) && (abs(digi.getTimeBin()-theTime)<= deltaT) ) {
    theLastChannel = iwg;
    wire_cluster.push_back( digi );
    return true;
  }

  return false;
}


/* findWireHitPosition
 *
 * This position is expressed in terms of wire #... is a float since it may be a fraction.
 */
float CSCHitFromWireOnly::findWireHitPosition() {
  
  // Again use center of mass to determine position of wire hit
  // To do so, need to know wire spacing and # of wires
  
  float y = 0.0;
  
  for ( unsigned i = 0; i < wire_cluster.size(); ++i ) {
    CSCWireDigi wdigi = wire_cluster[i];
    int wgroup = wdigi.getWireGroup();
    wire_in_cluster.push_back( wgroup );
    y += float( wgroup );
  }       

  float wiregpos = y /wire_cluster.size() ;

  return wiregpos;

}


/*  
 * Get wire groups which are part of ALCT(s) for this DetId (layer)
 */
std::vector<int> CSCHitFromWireOnly::getALCTWgroup( const CSCDetId& id, const CSCALCTDigiCollection* alcts ) {
  std::vector<int> wg;

  for ( CSCALCTDigiCollection::DigiRangeIterator it = alcts->begin(); it != alcts->end(); ++it ) {

    // Test if within same chamber
    // note that ALCT wiregroup is average over all 6 layers !!!
    const CSCDetId& alctId = (*it).first;
    if ( (alctId.chamber() == id.chamber()) &&
	 (alctId.station() == id.station()) &&
	 (alctId.ring()    == id.ring())    &&
	 (alctId.endcap()  == id.endcap())  ) {
      for ( std::vector<CSCALCTDigi>::const_iterator itr = (*it).second.first; itr != (*it).second.second; ++itr) {
	wg.push_back( int( itr->getKeyWG() ) );
      }
    }       
  }
  return wg;
}

/*  
 * Test if wire digi has matching ALCT digi
 */
bool  CSCHitFromWireOnly::foundALCTMatch( const CSCWireDigi& wdigi, std::vector<int> wgALCT ) {

  bool foundMatch = false;
  int wiregroup = wdigi.getWireGroup();

  // Note: ALCT id start at zero whereas wgroup # start at 1...
  for (unsigned i = 0; i < wgALCT.size(); ++i )
    if (abs(wiregroup - wgALCT[i] - 1) < 5 ) return true;

  return foundMatch;
}

