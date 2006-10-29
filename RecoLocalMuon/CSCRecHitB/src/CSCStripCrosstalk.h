#ifndef CSCRecHitB_CSCStripCrosstalk_h
#define CSCRecHitB_CSCStripCrosstalk_h

/** \class CSCStripCrosstalk
 *
 * This routine finds for a given DetId the X-talk.
 *
 * \author Dominique Fortin - UCR
 */

#include <CondFormats/CSCObjects/interface/CSCcrosstalk.h>
#include <CondFormats/DataRecord/interface/CSCcrosstalkRcd.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <vector>
#include <string>

class CSCStripCrosstalk
{
 public:

  /// configurable parameters
  explicit CSCStripCrosstalk( const edm::ParameterSet & ps );  
  ~CSCStripCrosstalk();

  // Member functions

  /// Load in the gains, X-talk and noise matrix and store in memory
  void setCrossTalk( const CSCcrosstalk* xtalk ) { xTalk = xtalk; }
 
  /// Get the Xtalks out of the database for each of the strips within layer.
  void getCrossTalk( const CSCDetId& id, float* m_left, float* b_left, float* m_right, float* b_right);

 private:

  bool isData;
  bool debug;
  CSCReadoutMappingFromFile theCSCMap;

  // Store in memory Gains, xtalks, and noise matrix
  const CSCcrosstalk     * xTalk;

};

#endif

