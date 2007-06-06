#ifndef CSCRecHitB_CSCFitXonStripWithGatti_h
#define CSCRecHitB_CSCFitXonStripWithGatti_h

/** \class CSCFitXonStripWithGatti
 *
 * When having both strip and wire hit in a layer, try using Gatti fitter to
 * improve position of strip hit.  If fit fails, use strip hit as is (using centroid).
 *
 * \author Dominique Fortin - UCR
 *
 * Based on work written by S. Durkin.  The following is an excerpt from S. Durkin's note
 *
 * The Gatti fit is applied on 3 strips x 3 time bins.  The auto-correlation
 * and cross-talks are taken into account, and the reconstructed signal is fitted to
 * the expected shape distribution.
 *
 * In order to perform the fit on 1 time bin, and 3 strips i = -1, 0, 1, a chi^2 minimization <br>
 * is performed of the type:                                                                  <br>
 *                                                                                            <br>
 * chi^2 = Sum_i ( q_i - N gatti_i(x))^2 / ( sigma_i(noise)^2 + sigma_i(systematic)^2 )       <br>
 *                                                                                            <br>
 * where N = Sum_i [ q_i gatti_i(x) ] / [ sigma_i(noise)^2 + sigma_i(systematic)^2 ]          <br>
 *           -----------------------------------------------------------------------          <br>
 *              Sum_i  gatti_i(x)^2 / [ sigma_i(noise)^2 + sigma_i(systematic)^2 ]            <br>
 *                                                                                            <br>
 * Since the fit is performed on 3 time bins, matrix algebra needs to be used and the above   <br>
 * expressions become                                                                         <br>
 *                                                                                            <br>
 * chi^2 = Sum_i S_i^(-1) D_i S_i^(-1, t)        where                                        <br>
 *                                                                                            <br>
 * S_i = S_i(noise) + S_i(systematics)                                                        <br>
 *                                                                                            <br>
 * S_i(noise) = sigma_i(t1,t2)^2  the auto-correlation noise matrix between time bin t1 & t2  <br>
 *                                                                                            <br>
 * S_i(systematics) = epsilon N_t1 N_t2    with epsilon ~ 1.5%                                <br>
 *                                                                                            <br>
 * The elements of the matrix D_i contains the difference between the expected and            <br>
 * measured charge such that the first element of the matrix is                               <br>
 *                                                                                            <br> 
 * D_i(t1,t1) = ( q_i,t1 - N_t1 gatti_i,t1(x) )^2                                             <br>
 */

#include <RecoLocalMuon/CSCRecHitB/interface/CSCStripHit.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <map>  
#include <vector>

class CSCLayer;
class CSCChamberSpecs;
class CSCFindPeakTime;
class CSCGains;
class CSCcrosstalk;
class CSCNoiseMatrix;
class CSCStripCrosstalk;
class CSCStripNoiseMatrix;

class CSCFitXonStripWithGatti
{
 public:

  explicit CSCFitXonStripWithGatti(const edm::ParameterSet& ps);

  ~CSCFitXonStripWithGatti();
  

  // Member functions
  
  /// Returns fitted local x position and its estimated error.
  void findXOnStrip( const CSCDetId& id, const CSCLayer* layer, const CSCStripHit& stripHit,
                     int centralStrip, float& xCentroid, float& stripWidth,
                     float& xGatti, float& tpeak, double& sigma, double& chisq );

  /// Use specs to setup Gatti parameters
  void initChamberSpecs();                       

  /// Perform fit by starting on either side of the central strip
  void runGattiFit( int istrt );

  /// Computes chi^2 by comparing expected charge distribution with measured
  float chisqrFromGatti( float x );

  /// Computes chi^2 by comparing expected charge distribution with measured (use x-talks + auto-correlation matrix)
  float chisqrFromGattiCalib( float x );

  /// Set matrix with autocorrelation to compute chi^2
  void setupMatrix();

  /// Evaluates the Gatti function for a given position on the strip without x-talks (deprecated)
  void getGatti( float x, int t );

  /// Evaluates the Gatti function for a given position on the strip and consider x-talks (deprecated)
  void getGattiCalib( float x, int t );


  /// Load in x-Talks and Noise Matrix
  void setCalibration( float GlobalGainAvg,
                       const CSCGains* gains,
                       const CSCcrosstalk* xtalk,
                       const CSCNoiseMatrix* noise ) {
    globalGainAvg = GlobalGainAvg;
    gains_ = gains;
    xtalk_ = xtalk;
    noise_ = noise;
  }

  
 private:


  double h;                                     // This is the distance between strip and wire planes
  float stripWidth;
  double r;                                     // This is the ratio h/stripwidth
  
  double k_1, k_2, k_3, sqrt_k_3, norm;         // See equation above for description
    
  // gatti fit data and error matrices
  float d[3][3];                                // 3x3 data array for gatti fit
  float q[3][3];                                // 3x3 theory array for gatti fit
  float f[3][3];                                // 3x3 fitted array for gatti

  /// x-talks  0 = left, 1 = middle, 2 = right ; and then second [] is for time bin tmax-1, tmax, tmax+1
  float xt_l[3][3], xt_r[3][3];
  float xt_lr0[3], xt_lr1[3], xt_lr2[3];

  /// Store elements of auto-correlation matrices:      0 = left, 1 = middle, 2 = right
  float a11[3], a12[3], a13[3], a22[3], a23[3], a33[3];

  /// Store elements of matrices for chi^2 computation: 0 = left, 1 = middle, 2 = right
  float v11[3], v12[3], v13[3], v22[3], v23[3], v33[3]; 

  // Parameters computed during gatti fit
  float x_gatti, dx_gatti, dxl_gatti, dxh_gatti, chi2_gatti;

  // Store chamber specs
  const CSCChamberSpecs* specs_;

  // Parameter settings from config file
  bool debug;
  bool useCalib;
  bool isData;
  bool use3TimeBins;
  double adcSystematics;
  float xtalksOffset;
  float xtalksSystematics;
  float minGattiStepSize;
  float minGattiError;

  /* Cache calibrations for current event
   *
   */
  float globalGainAvg;
  const CSCGains*       gains_;
  const CSCcrosstalk*   xtalk_;
  const CSCNoiseMatrix* noise_;

  // other classes used
  CSCStripCrosstalk*       stripCrosstalk_;
  CSCStripNoiseMatrix*     stripNoiseMatrix_;
  CSCFindPeakTime*         peakTimeFinder_;  

}; 

#endif
