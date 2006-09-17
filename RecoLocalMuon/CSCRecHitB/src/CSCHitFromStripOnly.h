#ifndef CSCRecHitB_CSCHitFromStripOnly_h
#define CSCRecHitB_CSCHitFromStripOnly_h

/** \class CSCHitFromStripOnly
 *
 * Search for strip with ADC output exceeding theThresholdForAPeak.  For each of these strips,
 * build a cluster of strip of size theClusterSize (typically 5 strips).  Finally, make
 * a Strip Hit out of these clusters by finding the center-of-mass position of the hit
 * The DetId, strip hit position, and peaking time are stored in a CSCStripHit collection.
 *
 * Here one has to be careful with the ME_1/a chambers:  in MC, digis are produced only for the first 16
 * strips, so one has to account for the ganging in groups of 3.
 *
 * In data, the ME_11 digis are stored in the same collection, so one has to untangle the output from
 * the ME_1a and ME_1b strips.  64 readouts from ME_1b, 16 from ME_1a.  Will have to figure out if ME_1a comes
 * first, and then the 64 ME_1b...
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
class CSCGains;
class CSCcrosstalk;
class CSCNoiseMatrix;
class CSCStripDigi;
class CSCPeakBinOfStripPulse;


class CSCHitFromStripOnly 
{
  
 public:

  typedef std::vector<CSCStripData> PulseHeightMap;
  
  explicit CSCHitFromStripOnly( const edm::ParameterSet& ps );
  
  ~CSCHitFromStripOnly();
  
  std::vector<CSCStripHit> runStrip( const CSCDetId& id, const CSCLayer* layer, const CSCStripDigiCollection::Range& rstripd);

  void setCalibration( const CSCGains* gains,
                       const CSCcrosstalk* xtalk,
                       const CSCNoiseMatrix* noise ) {
    gains_ = gains;
    xtalk_ = xtalk;
    noise_ = noise;
  }

  
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
  
  CSCStripData makeStripData( int centerStrip, int offset );

  bool isData;

  // Variables entering the CSCStripHit construction:
  int tmax_cluster;
  float t_peak;
  int ClusterSize;
  std::vector<float> strips_adc;  
  
  // The cuts for forming the strip hits are described in the data/.cfi file
  float theThresholdForAPeak;
  int theClusterSize;
  float theClusterChargeCut;
  bool stripHituse3timeBin;


  /*
   * Cache calibrations for current event
   */
  const CSCGains* gains_;
  const CSCcrosstalk* xtalk_;
  const CSCNoiseMatrix* noise_;
  
  CSCPeakBinOfStripPulse * pulseheightOnStripFinder_;


};

#endif

