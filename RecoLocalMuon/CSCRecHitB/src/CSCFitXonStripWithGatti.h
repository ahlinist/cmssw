#ifndef CSCRecHitB_CSCFitXonStripWithGatti_h
#define CSCRecHitB_CSCFitXonStripWithGatti_h

/** \class CSCFitXonStripWithGatti
 *
 * When having both strip and wire hit in a layer, try using Gatti fitter to
 * improve position of strip hit.  If fit fails, use strip hit as is.
 *
 * \author Dominique Fortin - UCR
 *
 * Based on binomial fitter written by S. Durkin
 */

#include <DataFormats/CSCRecHit/interface/CSCStripHit.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <map>  
#include <vector>

class CSCLayer;
class CSCChamberSpecs;

class CSCFitXonStripWithGatti
{
 public:

  explicit CSCFitXonStripWithGatti(const edm::ParameterSet& ps);

  ~CSCFitXonStripWithGatti();
  

  // Member functions
  
  /// Returns fitted local x position and its estimated error.
  void findXOnStrip( const CSCLayer* layer, const CSCStripHit& stripHit, 
                     float& xCentroid, float& stripWidth,
                     float& xGatti, double& sigma, double& chisq, double& prob );

  /// Use specs to setup Gatti parameters
  void initChamberSpecs();                       

  /// Perform fit by starting on either side of the central strip
  void runGattiFit( int istrt );

  /// Computes chi^2 by comparing expected charge distribution with measured
  float chisqrFromGatti( float x );

  /// Evaluates the Gatti function for a given position on the strip
  void getGatti( float x );

  
 private:

  double h;                                     // This is the distance between strip and wire planes
  float stripWidth;
  double k_1, k_2, k_3, sqrt_k_3, norm;         // See equation above for description
  
  
  // gatti fit data and error matrices
  float Q_tot;                                  // total sum of charge
  float d[3][3];                                // 3x3 data array for gatti fit
  float q[3][3];                                // 3x3 theory array for gatti fit
  float f[3][3];                                // 3x3 fitted array for gatti

  // Parameters computed during gatti fit
  float x_gatti, dx_gatti, dxl_gatti, dxh_gatti, chi2_gatti;


  // Store chamber specs
  const CSCChamberSpecs* specs_;

  
  // Parameter settings from config file
  bool debug;

}; 

#endif
