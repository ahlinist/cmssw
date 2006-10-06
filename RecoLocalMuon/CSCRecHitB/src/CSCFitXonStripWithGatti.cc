// This is CSCFitXonStripWithGatti.cc

#include <RecoLocalMuon/CSCRecHitB/src/CSCFitXonStripWithGatti.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCFindPeakTime.h>

#include <RecoLocalMuon/CSCRecHit/src/probab.h>

#include <DataFormats/CSCRecHit/interface/CSCStripHit.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/Exception.h>
                                                                                                 
#include <cmath>
#include <iostream>
                                                                                                 
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif
                                                                                                 

CSCFitXonStripWithGatti::CSCFitXonStripWithGatti(const edm::ParameterSet& ps){

  debug                      = ps.getUntrackedParameter<bool>("CSCDebug");
  use3TimeBins               = ps.getUntrackedParameter<bool>("CSCUse3x3Gatti");

  peakTimeFinder_            = new CSCFindPeakTime();

}

CSCFitXonStripWithGatti::~CSCFitXonStripWithGatti(){

//  delete peakTimeFinder_;

}


/* findPosition
 *
 */
void CSCFitXonStripWithGatti::findXOnStrip( const CSCLayer* layer, const CSCStripHit& stripHit, 
                                            float& xCenterStrip, float& sWidth,
                                            float& xGatti, double& sigma, double& chisq, double& prob ) { 

  // Initialize Gatti parameters using chamberSpecs
  // Cache specs_ info for ease of access
  specs_ = layer->chamber()->specs();
  stripWidth = sWidth;
  initChamberSpecs();


  // Initialize output parameters just in case the fit fails  
  xGatti = xCenterStrip;  
  sigma = prob = chisq= 9999.;

  int nStrips = stripHit.clusterSize();
  int CenterStrip = nStrips/2 + 1;   
  std::vector<float> adcs = stripHit.s_adc();
  int tmax = stripHit.tmax();

  // Loading in 3x3 matrix with corrected adcs..
  int j = 0;
  Q_tot[0] = 0.;
  Q_tot[1] = 0.;
  Q_tot[2] = 0.;
  
  for ( int i=1; i <= nStrips; i++ ) {
    if ( i > (CenterStrip-2) && i < (CenterStrip+2) ) {
      float adc[4];
      std::vector<float> adcsFit;
      for ( int t=0; t<4; t++ ) {
        int k = t + 3*(i-1);
        adc[t]    = adcs[k];
        d[j][t]   = adc[t];
        Q_tot[t] += adc[t];
      }
      // float tpeak;
      // peakTimeFinder_->FindPeakTime( tmax, adc, tpeak, adcsFit );
      // for ( int t=0; t<3; t++ ) {
      //  d[j][t]   = adcsFit[t];
      //  Q_tot[t] += adcsFit[t];
      // }
      j++;
    }
  }
    
  
  // Run Gatti for offset = 0
  runGattiFit( 0 );

  float tmp0_x      = x_gatti;
  float tmp0_chi2   = chi2_gatti;
  float tmp0_dl     = dxl_gatti;
  float tmp0_dh     = dxh_gatti;


  // Run Gatti for offset = 1
  runGattiFit( 1 );

  float tmp1_x      = x_gatti;
  float tmp1_chi2   = chi2_gatti;
    
  if ( fabs( tmp1_x - tmp0_x ) > 0.005 ) {
    if ( fabs( tmp1_chi2 - tmp0_chi2 ) > 3.){
      if ( tmp0_chi2 < tmp1_chi2 ){
        x_gatti    = tmp0_x;
	chi2_gatti = tmp0_chi2;
	dxl_gatti  = tmp0_dl;
	dxh_gatti  = tmp0_dh;
      }
    } else {
      x_gatti   = ( tmp0_x + tmp1_x ) / 2.;
      dxl_gatti = fabs( tmp0_x - tmp1_x )/ sqrt(12.);
      dxh_gatti = dxl_gatti;
    }
  }    

  float dx_gatti = ( dxl_gatti + dxh_gatti ) /2.;
  if ( dx_gatti < 0.003 ) dx_gatti = 0.003;

  xGatti = xCenterStrip - x_gatti * stripWidth;
  sigma = dx_gatti * stripWidth / sqrt(12.);      

  if (debug) {
        std::cout << "Output from Gatti:"        << std::endl;
        std::cout << "x      :  " << xGatti      << std::endl;
        std::cout << "chi^2  :  " << chi2_gatti  << std::endl;
  } 
}



/* runGattiFit
 *
 * Essentially, going from -1/2 strip to center of the strip and 
 * +1/2 of strip back towards center of the strip to determine 
 * where the best chi^2 is --> best x
 */
void CSCFitXonStripWithGatti::runGattiFit( int istrt ) {

  float dx;  
  if ( istrt == 0 ) {
    dx   = -0.5;
  } else {
    dx   =  0.5;
  } 
  float step = -1. * dx;
  
  float chi2 = 0.;
  float chi2min = 1.e12;
  float chi2last= 1.e12;

  for ( int istep = 0; istep < 10000; istep++ ) {  // Prevent from blowing up !
    
    chi2 = chisqrFromGatti( dx );
    
    if ( chi2 < chi2last ) {
      chi2last = chi2;
      if ( chi2min > chi2) chi2min = chi2;
      dx += step;
      continue;
    }
    
    if ( fabs(step) < 0.000002 ) break;
    
    dx = dx - 2. * step;
    step = step / 2.;
    chi2last = 1.e12;
  }
  
  dx = -dx;
 
  // Now compute errors
  float errl = .001;
  float errh = .001;

  
  // Look at chi^2 for dx - 0.001

  float dxl = dx - errl;
  float chi2l = chisqrFromGatti( dxl );
  chi2l = chi2l - chi2;
  if ( chi2l > 0. ) {
    errl = errl * sqrt( 1.0 / chi2l );
  } else {
    errl = 1000.;
  }


  // Look at chi^2 for dx + 0.001

  float dxh = errh - dx;
  float chi2h = chisqrFromGatti( dxh );
  chi2h = chi2h - chi2;
  if ( chi2h > 0. ) {
    errh = errh * sqrt( 1.0/chi2h );
  } else {
    errh = 1000.;
  }
 
  x_gatti    = dx;
  dxl_gatti  = errl;
  dxh_gatti  = errh;
  chi2_gatti = chi2;  
}



/* chisqrFromGatti
 *
 */
float CSCFitXonStripWithGatti::chisqrFromGatti( float x ) {

  // Compute Gatti function for 3 positions and 3 time bins
  for (int t = 0; t < 3; t++ ) getGatti( x, t );

  float chi2 = 0.;

  // What is the error on these adc counts after applying all the corrections (gains, noise cross talk) ?  
  // Blightly assume poisson stats ...

  // Also, leave the option open to use either 3 x 3 Gatti or simply look at 3 strip for tmax
  if (use3TimeBins) {
    for ( int i = 0; i < 3; i++) {
      for (int t = 0; t < 3; t++) {
        chi2 += (d[i][t] - q[i][t])*(d[i][t] - q[i][t]) / (d[i][t] + q[i][t]);
      } 
    }
  } else {
    int t = 1;
    for (int i = 0; i < 3; i++) {
      chi2 += (d[i][t] - q[i][t])*(d[i][t] - q[i][t]) / (d[i][t] + q[i][t]);
    } 
  }
  return chi2;
}



/* initChamberSpecs
 *
 */
void CSCFitXonStripWithGatti::initChamberSpecs() {

  // Distance between anode and cathode
  h = specs_->anodeCathodeSpacing();

  // Wire spacing
  double wspace = specs_->wireSpacing();

  // Wire radius
  double wradius = specs_->wireRadius();

  // Accepted parameters in Gatti function
  const double parm[5] = {.1989337e-02, -.6901542e-04, .8665786, 154.6177, -.680163e-03 };

  k_3 = ( parm[0]*wspace/h + parm[1] )
      * ( parm[2]*wspace/wradius + parm[3] + parm[4]*(wspace/wradius)*(wspace/wradius) );

  sqrt_k_3 = sqrt( k_3 );
  norm     = 0.5 / atan( sqrt_k_3 );
  k_2      = M_PI_2 * ( 1. - sqrt_k_3 /2. );
  k_1      = 0.25 * k_2 * sqrt_k_3 / atan( sqrt_k_3 );

}



/* getGatti
 *
 */
void CSCFitXonStripWithGatti::getGatti( float x, int t ) {

  double r = h / stripWidth;

  double g1 = Q_tot[t] * norm * r * atan( sqrt_k_3 * tanh( k_2 * (-x - 1.5)/r ) );
  double g2 = Q_tot[t] * norm * r * atan( sqrt_k_3 * tanh( k_2 * (-x - 0.5)/r ) );
  double g3 = Q_tot[t] * norm * r * atan( sqrt_k_3 * tanh( k_2 * (-x + 0.5)/r ) );
  double g4 = Q_tot[t] * norm * r * atan( sqrt_k_3 * tanh( k_2 * (-x + 1.5)/r ) );


//  if (debug) std::cout << "g3, g2, q[1][1] " << g3 << " " << g2 << " " << g3 - g2 << std::endl;

  q[0][t] = g2 - g1;
  q[1][t] = g3 - g2;
  q[2][t] = g4 - g3;

}
