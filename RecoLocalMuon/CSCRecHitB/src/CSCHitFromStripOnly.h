#ifndef CSCRecHitB_CSCHitFromStripOnly_h
#define CSCRecHitB_CSCHitFromStripOnly_h

/** \class CSCHitFromStripOnly
 *
 * Search for strip with ADC output exceeding theThresholdForAPeak.  For each of these strips,
 * build a cluster of strip of size theClusterSize (typically 5 strips).  Finally, make
 * a Strip Hit out of these clusters by finding the center-of-mass position of the hit
 * The DetId, strip hit position, and peaking time are stored in a CSCStripHit collection.
 *
 * \author Dominique Fortin - UCR
 *
 */
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripData.h>
#include <DataFormats/CSCRecHit/interface/CSCStripHit.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <vector>

class CSCLayer;
class CSCChamberSpecs;
class CSCLayerGeometry;
class CSCStripDigi;
class CSCPeakBinOfStripPulse;


class CSCHitFromStripOnly 
{
  
 public:
  typedef std::vector<CSCStripData> PulseHeightMap;
  
  explicit CSCHitFromStripOnly( const edm::ParameterSet& ps );
  
  ~CSCHitFromStripOnly();
  
  std::vector<CSCStripHit> runStrip( const CSCDetId& id, const CSCLayer* layer, const CSCStripDigiCollection::Range& rstripd);
  
 protected:
  
  /// Go through strip in layers and build a table with 
  void fillPulseHeights( const CSCStripDigiCollection::Range& );  
  /// Find local maxima
  void findMaxima();    
  /// Make clusters using local maxima
  float makeCluster( int centerStrip );
  std::vector<int> theMaxima;
  PulseHeightMap thePulseHeightMap;
  
  /// Find position of hit in strip cluster in terms of strip #
  float findHitOnStripPosition( const std::vector<CSCStripData>& data );
  /// Find peaking time for strip hit
  int findTmaxofCluster( const std::vector<CSCStripData>& data );
  
  CSCDetId id_;    
  const CSCLayer * layer_;
  const CSCLayerGeometry * layergeom_;
  const CSCChamberSpecs * specs_;
  
 private:
  
  CSCStripData makeStripData(int centerStrip, int offset, int ClusterSize);
  
  /// The cuts for forming the strip hits are described in the data/.cfi file
  int tmax_of_cluster;
  float theThresholdForAPeak;
  int theClusterSize;
  float theClusterChargeCut;
  bool stripHituse3timeBin;
  
  CSCPeakBinOfStripPulse * pulseheightOnStripFinder_;
};

#endif

