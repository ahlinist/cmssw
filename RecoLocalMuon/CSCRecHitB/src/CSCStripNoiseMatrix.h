#ifndef CSCRecHitB_CSCStripNoiseMatrix_h
#define CSCRecHitB_CSCStripNoiseMatrix_h

/** \class CSCStripNoiseMatrix
 *
 * This routine finds for a given DetId the autocorrelation noise in a given layer.
 * Note that the elements of the matrix have to be multiplied by the square of the
 * strip gain for that strip.
 *
 * \author Dominique Fortin - UCR
 */

#include <CondFormats/CSCObjects/interface/CSCNoiseMatrix.h>
#include <CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h>
#include <CondFormats/CSCObjects/interface/CSCGains.h>
#include <CondFormats/DataRecord/interface/CSCGainsRcd.h>
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
  void setNoiseMatrix( float GlobalGainAvg, 
                       const CSCGains* gains, 
                       const CSCNoiseMatrix* noise ) { 
    globalGainAvg = GlobalGainAvg;
    Gains = const_cast<CSCGains*> (gains);
    Noise = const_cast<CSCNoiseMatrix*> (noise); 
  }
 
  /// Get the noise matrix out of the database for each of the strips within a cluster.
  void getNoiseMatrix( const CSCDetId& id, int centralStrip, std::vector<float>& nMatrix );

  /// Get the gains out of the database for each of the strips within a cluster.
  float getStripGain( int chId, int thisStrip );

 private:

  bool debug;
  CSCReadoutMappingFromFile theCSCMap;
  int chamberIdPrefix;

  // Store in memory Gains and Noise matrix
  float globalGainAvg;
  CSCGains         * Gains;
  CSCNoiseMatrix   * Noise;

};

#endif

