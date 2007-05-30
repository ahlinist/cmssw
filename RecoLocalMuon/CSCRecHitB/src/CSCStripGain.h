#ifndef CSCRecHitB_CSCStripGain_h
#define CSCRecHitB_CSCStripGain_h

/** \class CSCCalibrateStrip
 *
 * This routine finds for a given DetId
 * the average global gain, the correction weight for the gains, 
 * in a given layer.  This is done such that it 
 * minimizes the number of calls to this class.
 *
 * \author Dominique Fortin - UCR
 */

#include <CondFormats/CSCObjects/interface/CSCGains.h>
#include <CondFormats/DataRecord/interface/CSCGainsRcd.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>


class CSCStripGain
{
 public:

  /// configurable parameters
  explicit CSCStripGain(const edm::ParameterSet & ps);  
  ~CSCStripGain();

  // Member functions

  /// Load in the gains, X-talk and noise matrix and store in memory
  void setCalibration( float GlobalGainAvg, const CSCGains* gains ) { 
    globalGainAvg = GlobalGainAvg;
    Gains_ = const_cast<CSCGains*> (gains); 
  }
 
  /// Get the gains out of the database for each of the strips within a cluster.
  void getStripGain( const CSCDetId& id, float* weights );

 private:

  bool debug;
  CSCReadoutMappingFromFile theCSCMap;
  int chamberIdPrefix;

  // Store in memory Gains
  float globalGainAvg;
  CSCGains         * Gains_;

};

#endif

