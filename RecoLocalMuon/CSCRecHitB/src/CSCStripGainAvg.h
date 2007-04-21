#ifndef CSCRecHitB_CSCStripGainAvg_h
#define CSCRecHitB_CSCStripGainAvg_h

/** \class CSCStripGainAvg
 *
 * This routine finds the average global gain for the whole CSC system, which is
 * needed to compute the correction weight for the strip gains.
 *
 * \author Dominique Fortin - UCR
 */

#include <CondFormats/CSCObjects/interface/CSCGains.h>
#include <CondFormats/DataRecord/interface/CSCGainsRcd.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>



class CSCStripGainAvg
{
 public:

  /// configurable parameters
  explicit CSCStripGainAvg(const edm::ParameterSet & ps);  
  ~CSCStripGainAvg();

  // Member functions

  /// Load in the gains, X-talk and noise matrix and store in memory
  void setCalibration( const CSCGains* gains ) { Gains = gains; }
 
  /// Computes the average gain for the whole CSC system.
  float getStripGainAvg();

 private:

  CSCReadoutMappingFromFile theCSCMap;

  // Store in memory Gains
  const CSCGains         * Gains;

};
#endif

