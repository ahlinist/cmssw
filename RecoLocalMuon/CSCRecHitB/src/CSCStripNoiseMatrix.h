#ifndef CSCRecHitB_CSCStripNoiseMatrix_h
#define CSCRecHitB_CSCStripNoiseMatrix_h

/** \class CSCStripNoiseMatrix
 *
 * This routine finds for a given DetId the autocorrelation noise in a given layer.
 *
 * \author Dominique Fortin - UCR
 */

#include "CondFormats/CSCObjects/interface/CSCNoiseMatrix.h"
#include "CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h"

#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <vector>
#include <string>


class CSCStripNoiseMatrix
{
 public:

  /// configurable parameters
  explicit CSCStripNoiseMatrix(const edm::ParameterSet & ps);  
  ~CSCStripNoiseMatrix();

  // Member functions

  /// Load in the noise matrix and store in memory
  void setNoiseMatrix( const CSCNoiseMatrix* noise ) { Noise = noise; }
 
  /// Get the noise matrix out of the database for each of the strips within a cluster.
 void getNoiseMatrix( const CSCDetId& id, std::vector<float>& nMatrix );

 private:

  bool isData;
  bool debug;
  CSCReadoutMappingFromFile theCSCMap;

  // Store in memory Noise matrix
  const CSCNoiseMatrix   * Noise;

};

#endif

