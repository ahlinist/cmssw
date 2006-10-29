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

#include <vector>
#include <string>


class CSCStripGain
{
 public:

  /// configurable parameters
  explicit CSCStripGain(const edm::ParameterSet & ps);  
  ~CSCStripGain();

  // Member functions

  /// Load in the gains, X-talk and noise matrix and store in memory
  void setCalibration( const CSCGains* gains ) { Gains = gains; }
 
  /// Computes the average gain for the whole CSC system.
  float getStripGainAvg();

  /// Get the gains out of the database for each of the strips within a cluster.
  void getStripGain( const CSCDetId& id, const float& globalGainAvg, float* weights );

 private:

  bool isData;
  bool debug;
  CSCReadoutMappingFromFile theCSCMap;

  // Store in memory Gains
  const CSCGains         * Gains;

};

#endif

