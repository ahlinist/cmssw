#ifndef CSCRecHitB_CSCHitFromWireOnly_h
#define CSCRecHitB_CSCHitFromWireOnly_h

/**
 * \class CSCHitFromWireOnly
 *
 * Search for hits within the wire groups.  For each of these hits, try forming clusters
 * by looking at adjacent wiregroup.  Form a wire Hit out of these clusters by finding 
 * the center-of-mass position of the hit in terms of the wire #.
 * The DetId, wire hit position, and peaking time are stored in a CSCWireHit collection.
 *
 *
 * To keep wire hits only so they can be used in segment building.
 * Only the DetId and wiregroup # are stored in a CSCWireHit collection
 *
 * \author Dominique Fortin - UCR
 *
 */

#include <RecoLocalMuon/CSCRecHitB/interface/CSCWireHit.h>

#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <vector>

class CSCLayer;
class CSCLayerGeometry;
class CSCDetId;


class CSCHitFromWireOnly 
{
 public:

  typedef std::vector<int> ChannelContainer;

  
  explicit CSCHitFromWireOnly( const edm::ParameterSet& ps );
  
  ~CSCHitFromWireOnly();
  
  std::vector<CSCWireHit> runWire( const CSCDetId& id, const CSCLayer* layer, const CSCWireDigiCollection::Range& rwired, const CSCALCTDigiCollection* alcts ); 
  void makeWireCluster(const CSCWireDigi& digi);
  bool addToCluster(const CSCWireDigi& digi); 
  float findWireHitPosition();
  
  const CSCLayer * layer_;
  const CSCLayerGeometry * layergeom_;
  
 private:

  /// Get wire groups which are part of ALCT(s) for this DetId (layer)
  std::vector<int> getALCTWgroup( const CSCDetId& id, const CSCALCTDigiCollection* alcts );
   
  /// Check if found matching between ALCT trigger and wiregroup
  bool foundALCTMatch( const CSCWireDigi& digi, std::vector<int> wgALCT );


  std::vector<CSCWireDigi> wire_cluster;
  std::vector<int> wire_in_cluster;
  std::vector<float> wire_spacing;
  int theTime;
  int theLastChannel;
  
  bool debug;
  int deltaT;
  int clusterSize;
  bool useCleanWireCollection;
};

#endif


