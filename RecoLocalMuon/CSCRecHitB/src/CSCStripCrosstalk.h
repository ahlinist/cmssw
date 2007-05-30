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
  void setCrossTalk( const CSCcrosstalk* xtalk ) { 
    xTalk_ = const_cast<CSCcrosstalk*> (xtalk); 
  }
 
  /// Get the Xtalks out of the database for each of the strips within layer.
  void getCrossTalk( const CSCDetId& id, int centralStrip, std::vector<float>& xtalks);

 private:

  bool debug;
  CSCReadoutMappingFromFile theCSCMap;
  int chamberIdPrefix; 

  // Store in memory xtalks
  CSCcrosstalk     * xTalk_;
};

#endif

