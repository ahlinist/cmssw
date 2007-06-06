// This is CSCFitXonStripWithGatti.cc

#include <RecoLocalMuon/CSCRecHitB/src/CSCFitXonStripWithGatti.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripCrosstalk.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripNoiseMatrix.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCFindPeakTime.h>
#include <RecoLocalMuon/CSCRecHitB/src/probab.h>
#include <RecoLocalMuon/CSCRecHitB/interface/CSCStripHit.h>

#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>

#include <CondFormats/CSCObjects/interface/CSCGains.h>
#include <CondFormats/DataRecord/interface/CSCGainsRcd.h>
#include <CondFormats/CSCObjects/interface/CSCcrosstalk.h>
#include <CondFormats/DataRecord/interface/CSCcrosstalkRcd.h>
#include <CondFormats/CSCObjects/interface/CSCNoiseMatrix.h> 
#include <CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h>


#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/Exception.h>

#include <vector>
#include <cmath>
#include <iostream>
                                                                                                 
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif
                                                                                                 

CSCFitXonStripWithGatti::CSCFitXonStripWithGatti(const edm::ParameterSet& ps){

  debug                      = ps.getUntrackedParameter<bool>("CSCDebug");
  useCalib                   = ps.getUntrackedParameter<bool>("CSCUseCalibrations");
  adcSystematics             = ps.getUntrackedParameter<double>("CSCCalibrationSystematics");        
  xtalksOffset               = ps.getUntrackedParameter<double>("CSCStripxtalksOffset");
  xtalksSystematics          = ps.getUntrackedParameter<double>("CSCStripxtalksSystematics");
  minGattiStepSize           = ps.getUntrackedParameter<double>("CSCminGattiStepSize");
  minGattiError              = ps.getUntrackedParameter<double>("CSCminGattiError");
  stripCrosstalk_            = new CSCStripCrosstalk( ps );
  stripNoiseMatrix_          = new CSCStripNoiseMatrix( ps );

  peakTimeFinder_            = new CSCFindPeakTime();
}


CSCFitXonStripWithGatti::~CSCFitXonStripWithGatti(){
  delete stripCrosstalk_;
  delete stripNoiseMatrix_;
  delete peakTimeFinder_;
}


/* findPosition
 *
 */
void CSCFitXonStripWithGatti::findXOnStrip( const CSCDetId& id, const CSCLayer* layer, const CSCStripHit& stripHit, 
                                            int centralStrip, float& xCenterStrip, float& sWidth, 
                                            float& xGatti, float& tpeak, double& sigma, double& chisq ) {

  // Initialize Gatti parameters using chamberSpecs
  // Cache specs_ info for ease of access
  specs_ = layer->chamber()->specs();
  stripWidth = sWidth;
  initChamberSpecs();

  int ring = id.ring();

  // Initialize output parameters just in case the fit fails  
  xGatti = xCenterStrip;  
  sigma = chisq = 9999.;

  CSCStripHit::ChannelContainer strips = stripHit.strips();
  int nStrips = strips.size();
  int CenterStrip = nStrips/2 + 1;   
  std::vector<float> adcs = stripHit.s_adc();
  int tmax = stripHit.tmax();

  // Fit peaking time only if using calibrations
  float t_peak = tpeak;
  float t_zero = 0.;
  float adc[4];

  if ( useCalib ) {
    bool useFittedCharge = false;
    for ( int t = 0; t < 4; ++t ) {
      int k  = t + 4 * (CenterStrip-1);
      adc[t] = adcs[k];
    }
    useFittedCharge = peakTimeFinder_->FindPeakTime( tmax, adc, t_zero, t_peak );  // Clumsy...  can remove t_peak ?
    tpeak = t_peak+t_zero;
  }
      
  // Now fill with array for Gatti fitting
  int j = 0;
  for ( int i = 1; i <= nStrips; ++i ) {
    if ( i > (CenterStrip-2) && i < (CenterStrip+2) ) {
      std::vector<float> adcsFit;
      for ( int t = 0; t < 4; ++t ) {
        int k  = t + 4*(i-1);
        adc[t] = adcs[k];
        if ( t < 3) d[j][t] = adc[t];
      }
      j++;
    }
  }


  // Load in x-talks:

  if ( useCalib ) {
    std::vector<float> xtalks;
    stripCrosstalk_->setCrossTalk( xtalk_ );
    stripCrosstalk_->getCrossTalk( id, centralStrip, xtalks);
    float dt = 50. * tmax - (t_peak + t_zero);  // QUESTION:  should it be only - (t_peak) ???

    for ( int t = 0; t < 3; ++t ) {
      xt_l[0][t] = xtalks[0] * (50.* (t-1) + dt) + xtalks[1] + xtalksOffset;
      xt_r[0][t] = xtalks[2] * (50.* (t-1) + dt) + xtalks[3] + xtalksOffset;
      xt_l[1][t] = xtalks[4] * (50.* (t-1) + dt) + xtalks[5] + xtalksOffset;
      xt_r[1][t] = xtalks[6] * (50.* (t-1) + dt) + xtalks[7] + xtalksOffset;
      xt_l[2][t] = xtalks[8] * (50.* (t-1) + dt) + xtalks[9] + xtalksOffset;
      xt_r[2][t] = xtalks[10]* (50.* (t-1) + dt) + xtalks[11]+ xtalksOffset;

      xt_lr0[t] = (1. - xt_l[0][t] - xt_r[0][t]);
      xt_lr1[t] = (1. - xt_l[1][t] - xt_r[1][t]);
      xt_lr2[t] = (1. - xt_l[2][t] - xt_r[2][t]);
    }
  } else { 
    for ( int t = 0; t < 3; ++t ) {
      xt_l[0][t] = xtalksOffset;
      xt_r[0][t] = xtalksOffset;
      xt_l[1][t] = xtalksOffset; 
      xt_r[1][t] = xtalksOffset; 
      xt_l[2][t] = xtalksOffset; 
      xt_r[2][t] = xtalksOffset; 

      xt_lr0[t] = (1. - xt_l[0][t] - xt_r[0][t]);
      xt_lr1[t] = (1. - xt_l[1][t] - xt_r[1][t]);
      xt_lr2[t] = (1. - xt_l[2][t] - xt_r[2][t]);
    } 
  }
  

  // vector containing noise starts at tmax - 1, and tmax > 3, but....
  int tbin = tmax - 4;

  // .... originally, suppose to have tmax in tbin 4 or 5, but noticed in MTCC lots of 
  // hits with tmax == 3, so let's allow these, and shift down noise matrix by one element...
  // This is a patch because the calibration database doesn't have elements for tbin = 2, 
  // e.g. there is no element e[tmax-1,tmax+1] = e[2,4].

  if (tmax < 4) tbin = 0;    // patch

  // Load in auto-correlation noise matrices
  if ( useCalib ) {
    std::vector<float> nmatrix;
    stripNoiseMatrix_->setNoiseMatrix( globalGainAvg, gains_, noise_ );
    stripNoiseMatrix_->getNoiseMatrix( id, centralStrip, nmatrix);

    for ( int istrip =0; istrip < 3; ++istrip ) {
      a11[istrip] = nmatrix[0+tbin*3+istrip*15];
      a12[istrip] = nmatrix[1+tbin*3+istrip*15];
      a13[istrip] = nmatrix[2+tbin*3+istrip*15];
      a22[istrip] = nmatrix[3+tbin*3+istrip*15];
      a23[istrip] = nmatrix[4+tbin*3+istrip*15];
      a33[istrip] = nmatrix[6+tbin*3+istrip*15];
    }
  } else {
    // FIXME:  NO HARD WIRED VALUES !!!
    for ( int istrip =0; istrip < 3; ++istrip ) {
      a11[istrip] = 10.0;
      a12[istrip] = 0.0;
      a13[istrip] = 0.0;
      a22[istrip] = 10.0;
      a23[istrip] = 0.0;
      a33[istrip] = 10.0;
    }
  }
  
  // Set Matrix used in Gatti fitting
  setupMatrix();

  // Run Gatti for offset = 0
  runGattiFit( 0 );

//  if (ring < 2 ) std::cout << "Gatti_0  " << x_gatti << " " << chi2_gatti << " " << dxl_gatti << " " << dxh_gatti << std::endl;
  
  float tmp0_x      = x_gatti;
  float tmp0_chi2   = chi2_gatti;
  float tmp0_dl     = dxl_gatti;
  float tmp0_dh     = dxh_gatti;

  // Run Gatti for offset = 1
  runGattiFit( 1 );

//  if (ring < 2 ) std::cout << "Gatti_1  " << x_gatti << " " << chi2_gatti << " " << dxl_gatti << " " << dxh_gatti << std::endl;

  float tmp1_x      = x_gatti;
  float tmp1_chi2   = chi2_gatti;
  float tmp1_dl     = dxl_gatti;
  float tmp1_dh     = dxh_gatti;

  float test        = fabs( tmp1_x - tmp0_x );

  if ( test < 2.1 * minGattiStepSize ) {
    // Two fits are close to one another
    x_gatti    = ( tmp0_x + tmp1_x ) / 2.;
    dxl_gatti  = ( tmp0_dl + tmp1_dl ) / 2.;  //  equivalent of * 1/sqrt(12)
    dxh_gatti  = ( tmp0_dh + tmp1_dh ) / 2.;
    chi2_gatti = ( tmp1_chi2 + tmp0_chi2 ) / 2.;
  }
  else if ( test < 0.1 ) {
    // One of the fits didn't fit as well.
    if ( tmp0_chi2 < tmp1_chi2 ){
      x_gatti    = tmp0_x;
      chi2_gatti = tmp0_chi2;
      dxl_gatti  = tmp0_dl;
      dxh_gatti  = tmp0_dh;
    } else {
      x_gatti    = tmp1_x;
      chi2_gatti = tmp1_chi2;
      dxl_gatti  = tmp1_dl;
      dxh_gatti  = tmp1_dh;
    }  
  } else {  
    // One of the fits failed completely:  set to center of strip
    x_gatti    = 0.;      
    dxl_gatti  = 0.2887;  // This is 1/sqrt(12)
    dxh_gatti  = dxl_gatti;
    chi2_gatti = 9999.;
  }

  float dx_gatti = ( dxl_gatti + dxh_gatti ) /2.;

  if ( dx_gatti < minGattiError ) dx_gatti =  minGattiError;
  if ( dx_gatti > 0.2887 ) dx_gatti =  0.2887; // this is 1/sqrt(12)

//  if ( ring < 2 )  std::cout << "GattiFit " << x_gatti << " " << chi2_gatti << " " << dx_gatti << std::endl;


  xGatti = xCenterStrip - x_gatti * stripWidth;
  sigma  = dx_gatti * stripWidth;      
  chisq  = chi2_gatti;
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

  // This is using the full calibration method 
  if (useCalib) {

    while ( true ) {    

      chi2 = chisqrFromGattiCalib( dx );

      if ( chi2 < chi2last ) {
        chi2last = chi2;
        if ( chi2min > chi2) chi2min = chi2;
        dx += step;
        continue;
      }

      if ( fabs(step) < minGattiStepSize ) break;
        
      dx = dx - 2. * step;
      step = step / 2.;
      chi2last = 1.e12;
    }

  // Don't use the calibrations
  } else {

    while ( true ) {    

      chi2 = chisqrFromGattiCalib( dx );

      if ( chi2 < chi2last ) {
        chi2last = chi2;
        if ( chi2min > chi2) chi2min = chi2;
        dx += step;
        continue;
      }

      if ( fabs(step) < minGattiStepSize ) break;
        
      dx = dx - 2. * step;
      step = step / 2.;
      chi2last = 1.e12;
    }
  }
    
  dx = -dx;
 
  // Now compute errors --> What's the proper interval to look at ???
  // Picked the values below which correspond to the 1-sigma resolution observed in MC
  float errl = 0.02;
  float errh = 0.02;
  
  // Look at chi^2 for dx - errl
  float dxl = -dx - errl;
  float chi2l = chisqrFromGatti( dxl );
  chi2l = fabs(chi2l - chi2);
  if ( chi2l != 0. ) {
    errl = errl * sqrt( 1.0 / chi2l );
  } else {
    errl = 1000.;
  }


  // Look at chi^2 for dx + errh
  float dxh = errh - dx;
  float chi2h = chisqrFromGatti( dxh );
  chi2h = fabs(chi2h - chi2);
  if ( chi2h != 0. ) {
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

  float chi2, dd;
  float sn11,sn22,sn33;
  float sd11,sd22,sd33;
  float sn1,sn2,sn3,n1,n2,n3;

  // Compute Gatti function for 3 positions and 3 time bins
  //  getGatti( x, 0 ); // deprecated

  double   g1 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x - 1.5)/r ) );
  double   g2 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x - 0.5)/r ) );
  double   g3 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x + 0.5)/r ) );
  double   g4 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x + 1.5)/r ) );

  q[0][0] = q[0][1] = q[0][2] = g2 - g1; 
  q[1][0] = q[1][1] = q[1][2] = g3 - g2;
  q[2][0] = q[2][1] = q[2][2] = g4 - g3;


  sn11 = v11[0]*q[0][0]*d[0][0] + v11[1]*q[1][0]*d[1][0] + v11[2]*q[2][0]*d[2][0];
  sd11 = v11[0]*q[0][0]*q[0][0] + v11[1]*q[1][0]*q[1][0] + v11[2]*q[2][0]*q[2][0];
  
  sn22 = v22[0]*q[0][1]*d[0][1] + v22[1]*q[1][1]*d[1][1] + v22[2]*q[2][1]*d[2][1];
  sd22 = v22[0]*q[0][1]*q[0][1] + v22[1]*q[1][1]*q[1][1] + v22[2]*q[2][1]*q[2][1];
  
  sn33 = v33[0]*q[0][2]*d[0][2] + v33[1]*q[1][2]*d[1][2] + v33[2]*q[2][2]*d[2][2];
  sd33 = v33[0]*q[0][2]*q[0][2] + v33[1]*q[1][2]*q[1][2] + v33[2]*q[2][2]*q[2][2];

  sn1 = sn11;
  sn2 = sn22;
  sn3 = sn33;

/* These are the normalization factors, solutions to:
 *     sd11*N1+sd21*N2+sd31*N3-sn1=0
 *     sd12*N1+sd22*N2+sd32*N3-sn2=0
 *     sd13*N1+sd23*N2+sd33*N3-sn3=0
 */

  dd = (sd11*sd22*sd33);
  n1 = (sn1*sd22*sd33)/dd;
  n2 = (sd11*sd33*sn2)/dd;
  n3 = (sd11*sd22*sn3)/dd;
      
  // Now compute chi^2
  chi2  = 0.0;

  chi2 +=        v11[0] * (d[0][0] - n1*q[0][0]) * (d[0][0] - n1*q[0][0]) 
               + v11[1] * (d[1][0] - n1*q[1][0]) * (d[1][0] - n1*q[1][0])
               + v11[2] * (d[2][0] - n1*q[2][0]) * (d[2][0] - n1*q[2][0]);

  chi2 +=        v22[0] * (d[0][1] - n2*q[0][1]) * (d[0][1] - n2*q[0][1])
               + v22[1] * (d[1][1] - n2*q[1][1]) * (d[1][1] - n2*q[1][1])
               + v22[2] * (d[2][1] - n2*q[2][1]) * (d[2][1] - n2*q[2][1]);

  chi2 +=        v33[0] * (d[0][2] - n3*q[0][2]) * (d[0][2] - n3*q[0][2])
               + v33[1] * (d[1][2] - n3*q[1][2]) * (d[1][2] - n3*q[1][2])
               + v33[2] * (d[2][2] - n3*q[2][2]) * (d[2][2] - n3*q[2][2]);

  return chi2;
}


/* chisqrFromGattiCalib
 *
 */
float CSCFitXonStripWithGatti::chisqrFromGattiCalib( float x ) {

  float chi2, dd;
  float sn11,sn12,sn13,sn21,sn22,sn23,sn31,sn32,sn33;
  float sd11,sd12,sd13,sd21,sd22,sd23,sd31,sd32,sd33;
  float sn1,sn2,sn3,n1,n2,n3;

  // Compute Gatti function for 3 positions and 3 time bins
  //  getGatti( x, 0 );  // deprecated

  double   g0 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x - 2.5)/r ) );
  double   g1 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x - 1.5)/r ) );
  double   g2 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x - 0.5)/r ) );
  double   g3 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x + 0.5)/r ) );
  double   g4 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x + 1.5)/r ) );
  double   g5 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x + 2.5)/r ) ); 

   // Now correct for x-talks:
  for (int t = 0; t < 3; ++t) {
    q[0][t] = (g2 - g1) * xt_lr0[t] + (g1 - g0) * xt_r[0][t] + (g3 - g2) * xt_l[1][t]; 
    q[1][t] = (g3 - g2) * xt_lr1[t] + (g2 - g1)  * xt_r[0][t] +(g4 - g3) * xt_l[2][t];
    q[2][t] = (g4 - g3) * xt_lr2[t] + (g3 - g2) * xt_r[1][t] + (g5 - g4) * xt_l[2][t];
  }

     sn11 = v11[0]*q[0][0]*d[0][0] + v11[1]*q[1][0]*d[1][0] + v11[2]*q[2][0]*d[2][0];
     sd11 = v11[0]*q[0][0]*q[0][0] + v11[1]*q[1][0]*q[1][0] + v11[2]*q[2][0]*q[2][0];
     sn12 = v12[0]*q[0][0]*d[0][1] + v12[1]*q[1][0]*d[1][1] + v12[2]*q[2][0]*d[2][1];
     sd12 = v12[0]*q[0][0]*q[0][1] + v12[1]*q[1][0]*q[1][1] + v12[2]*q[2][0]*q[2][1];
     sn13 = v13[0]*q[0][0]*d[0][2] + v13[1]*q[1][0]*d[1][2] + v13[2]*q[2][0]*d[2][2];
     sd13 = v13[0]*q[0][0]*q[0][2] + v13[1]*q[1][0]*q[1][2] + v13[2]*q[2][0]*q[2][2];
  
     sn21 = v12[0]*q[0][1]*d[0][0] + v12[1]*q[1][1]*d[1][0] + v12[2]*q[2][1]*d[2][0];
     sd21 = v12[0]*q[0][1]*q[0][0] + v12[1]*q[1][1]*q[1][0] + v12[2]*q[2][1]*q[2][0];
     sn22 = v22[0]*q[0][1]*d[0][1] + v22[1]*q[1][1]*d[1][1] + v22[2]*q[2][1]*d[2][1];
     sd22 = v22[0]*q[0][1]*q[0][1] + v22[1]*q[1][1]*q[1][1] + v22[2]*q[2][1]*q[2][1];
     sn23 = v23[0]*q[0][1]*d[0][2] + v23[1]*q[1][1]*d[1][2] + v23[2]*q[2][1]*d[2][2];
     sd23 = v23[0]*q[0][1]*q[0][2] + v23[1]*q[1][1]*q[1][2] + v23[2]*q[2][1]*q[2][2];
  
     sn31 = v13[0]*q[0][2]*d[0][0] + v13[1]*q[1][2]*d[1][0] + v13[2]*q[2][2]*d[2][0];
     sd31 = v13[0]*q[0][2]*q[0][0] + v13[1]*q[1][2]*q[1][0] + v13[2]*q[2][2]*q[2][0];
     sn32 = v23[0]*q[0][2]*d[0][1] + v23[1]*q[1][2]*d[1][1] + v23[2]*q[2][2]*d[2][1];
     sd32 = v23[0]*q[0][2]*q[0][1] + v23[1]*q[1][2]*q[1][1] + v23[2]*q[2][2]*q[2][1];
     sn33 = v33[0]*q[0][2]*d[0][2] + v33[1]*q[1][2]*d[1][2] + v33[2]*q[2][2]*d[2][2];
     sd33 = v33[0]*q[0][2]*q[0][2] + v33[1]*q[1][2]*q[1][2] + v33[2]*q[2][2]*q[2][2];

  sn1 = sn11 + sn12 + sn13;
  sn2 = sn21 + sn22 + sn23;
  sn3 = sn31 + sn32 + sn33;

/* These are the normalization factors, solutions to:
 *     sd11*N1+sd21*N2+sd31*N3-sn1=0
 *     sd12*N1+sd22*N2+sd32*N3-sn2=0
 *     sd13*N1+sd23*N2+sd33*N3-sn3=0
 */

  dd = (-sd11*sd23*sd32 + sd11*sd22*sd33 + sd13*sd21*sd32 - sd22*sd13*sd31 + sd23*sd12*sd31 - sd12*sd21*sd33);
  n1 = (-sd21*sd33*sn2  + sd21*sn3*sd32  - sd31*sd22*sn3  + sd31*sd23*sn2  - sn1*sd23*sd32  + sn1*sd22*sd33)/dd;
  n2 =-(-sd11*sd33*sn2  + sd11*sn3*sd32  + sd13*sd31*sn2  + sd33*sd12*sn1  - sn3*sd12*sd31  - sd13*sn1*sd32)/dd;
  n3 = (-sd22*sd13*sn1  + sd11*sd22*sn3  - sd11*sd23*sn2  + sd23*sd12*sn1  + sd13*sd21*sn2  - sd12*sd21*sn3)/dd;
      
  // Now compute chi^2
  chi2  = 0.0;

   chi2 +=       v11[0] * (d[0][0] - n1*q[0][0]) * (d[0][0] - n1*q[0][0]) 
               + v11[1] * (d[1][0] - n1*q[1][0]) * (d[1][0] - n1*q[1][0])
               + v11[2] * (d[2][0] - n1*q[2][0]) * (d[2][0] - n1*q[2][0]);

   chi2 += 2.* ( v12[0] * (d[0][0] - n1*q[0][0]) * (d[0][1] - n2*q[0][1])
               + v12[1] * (d[1][0] - n1*q[1][0]) * (d[1][1] - n2*q[1][1])
               + v12[2] * (d[2][0] - n1*q[2][0]) * (d[2][1] - n2*q[2][1]) );

   chi2 += 2.* ( v13[0] * (d[0][0] - n1*q[0][0]) * (d[0][2] - n3*q[0][2])
               + v13[1] * (d[1][0] - n1*q[1][0]) * (d[1][2] - n3*q[1][2])
               + v13[2] * (d[2][0] - n1*q[2][0]) * (d[2][2] - n3*q[2][2]) );

   chi2 +=       v22[0] * (d[0][1] - n2*q[0][1]) * (d[0][1] - n2*q[0][1])
               + v22[1] * (d[1][1] - n2*q[1][1]) * (d[1][1] - n2*q[1][1])
               + v22[2] * (d[2][1] - n2*q[2][1]) * (d[2][1] - n2*q[2][1]);

   chi2 += 2.* ( v23[0] * (d[0][1] - n2*q[0][1]) * (d[0][2] - n3*q[0][2])
               + v23[1] * (d[1][1] - n2*q[1][1]) * (d[1][2] - n3*q[1][2])
               + v23[2] * (d[2][1] - n2*q[2][1]) * (d[2][2] - n3*q[2][2]) ); 

   chi2 +=       v33[0] * (d[0][2] - n3*q[0][2]) * (d[0][2] - n3*q[0][2])
               + v33[1] * (d[1][2] - n3*q[1][2]) * (d[1][2] - n3*q[1][2])
               + v33[2] * (d[2][2] - n3*q[2][2]) * (d[2][2] - n3*q[2][2]);

  return chi2;
}


/* setupMatrix
 *
 */
void CSCFitXonStripWithGatti::setupMatrix() {

  double dd, a11t, a12t, a13t, a22t, a23t, a33t;
  double syserr = adcSystematics;
  double xtlk_err = xtalksSystematics;

  // Left strip
  a11t = a11[0] + syserr*syserr * d[0][0]*d[0][0] + xtlk_err*xtlk_err*d[1][0]*d[1][0];
  a12t = a12[0] + syserr*syserr * d[0][0]*d[0][1];
  a13t = a13[0] + syserr*syserr * d[0][0]*d[0][2];
  a22t = a22[0] + syserr*syserr * d[0][1]*d[0][1] + xtlk_err*xtlk_err*d[1][1]*d[1][1];
  a23t = a23[0] + syserr*syserr * d[0][1]*d[0][2];
  a33t = a33[0] + syserr*syserr * d[0][2]*d[0][2] + xtlk_err*xtlk_err*d[1][2]*d[1][2];

  dd     = (a11t*a33t*a22t - a11t*a23t*a23t - a33t*a12t*a12t 
                       + 2.* a12t*a13t*a23t - a13t*a13t*a22t );

  v11[0] = (a33t*a22t - a23t*a23t)/dd;
  v12[0] =-(a33t*a12t - a13t*a23t)/dd;
  v13[0] = (a12t*a23t - a13t*a22t)/dd;
  v22[0] = (a33t*a11t - a13t*a13t)/dd;
  v23[0] =-(a23t*a11t - a12t*a13t)/dd;
  v33[0] = (a22t*a11t - a12t*a12t)/dd;
     
  // Center strip
  a11t = a11[1] + syserr*syserr * d[1][0]*d[1][0] + xtlk_err*xtlk_err*(d[0][0]*d[0][0]+d[2][0]*d[2][0]);
  a12t = a12[1] + syserr*syserr * d[1][0]*d[1][1];
  a13t = a13[1] + syserr*syserr * d[1][0]*d[1][2];
  a22t = a22[1] + syserr*syserr * d[1][1]*d[1][1] + xtlk_err*xtlk_err*(d[0][1]*d[0][1]+d[2][1]*d[2][1]);
  a23t = a23[1] + syserr*syserr * d[1][1]*d[1][2];
  a33t = a33[1] + syserr*syserr * d[1][2]*d[1][2] + xtlk_err*xtlk_err*(d[0][2]*d[0][2]+d[2][2]*d[2][2]);

  dd     = (a11t*a33t*a22t - a11t*a23t*a23t - a33t*a12t*a12t
                       + 2.* a12t*a13t*a23t - a13t*a13t*a22t );

  v11[1] = (a33t*a22t - a23t*a23t)/dd;
  v12[1] =-(a33t*a12t - a13t*a23t)/dd;
  v13[1] = (a12t*a23t - a13t*a22t)/dd;
  v22[1] = (a33t*a11t - a13t*a13t)/dd;
  v23[1] =-(a23t*a11t - a12t*a13t)/dd;
  v33[1] = (a22t*a11t - a12t*a12t)/dd;

  // Right strip
  a11t = a11[2] + syserr*syserr * d[2][0]*d[2][0] + xtlk_err*xtlk_err*d[1][0]*d[1][0];
  a12t = a12[2] + syserr*syserr * d[2][0]*d[2][1];
  a13t = a13[2] + syserr*syserr * d[2][0]*d[2][2];
  a22t = a22[2] + syserr*syserr * d[2][1]*d[2][1] + xtlk_err*xtlk_err*d[1][1]*d[1][1];
  a23t = a23[2] + syserr*syserr * d[2][1]*d[2][2];
  a33t = a33[2] + syserr*syserr * d[2][2]*d[2][2] + xtlk_err*xtlk_err*d[1][2]*d[1][2];

  dd     = (a11t*a33t*a22t - a11t*a23t*a23t - a33t*a12t*a12t
                        +2.* a12t*a13t*a23t - a13t*a13t*a22t );

  v11[2] = (a33t*a22t - a23t*a23t)/dd;
  v12[2] =-(a33t*a12t - a13t*a23t)/dd;
  v13[2] = (a12t*a23t - a13t*a22t)/dd;
  v22[2] = (a33t*a11t - a13t*a13t)/dd;
  v23[2] =-(a23t*a11t - a12t*a13t)/dd;
  v33[2] = (a22t*a11t - a12t*a12t)/dd;
}


/* initChamberSpecs
 *
 */
void CSCFitXonStripWithGatti::initChamberSpecs() {

  // Distance between anode and cathode
  h = specs_->anodeCathodeSpacing();
  r = h / stripWidth;

  // Wire spacing
  double wspace = specs_->wireSpacing();

  // Wire radius
  double wradius = specs_->wireRadius();

  // Accepted parameters in Gatti function
  const double parm[5] = {.1989337e-02, -.6901542e-04, .8665786, 154.6177, -.680163e-03 };

  k_3 = ( parm[0]*wspace/h + parm[1] )
      * ( parm[2]*wspace/wradius + parm[3] + parm[4]*(wspace/wradius)*(wspace/wradius) );

  sqrt_k_3 = sqrt( k_3 );
  norm     = r * (0.5 / atan( sqrt_k_3 )); // changed from norm to r * norm
  k_2      = M_PI_2 * ( 1. - sqrt_k_3 /2. );
  k_1      = 0.25 * k_2 * sqrt_k_3 / atan( sqrt_k_3 );
}


/* getGatti
 *
 * Compute expected charge for a given x and time bin
 */
void CSCFitXonStripWithGatti::getGatti( float x, int t_disabled ) {

  std::cout << "This should not be called anymore !!!!!!!!!!!!!" << std::endl;  
  return;
  double   g1 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x - 1.5)/r ) );
  double   g2 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x - 0.5)/r ) );
  double   g3 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x + 0.5)/r ) );
  double   g4 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x + 1.5)/r ) );
  
  q[0][0] = q[0][1] = q[0][2] = g2 - g1;
  q[1][0] = q[1][1] = q[1][2] = g3 - g2;
  q[2][0] = q[2][1] = q[2][2] = g4 - g3;


}


/* getGattiCalib
 *
 * Compute expected charge for a given x and time bin using x-talks !
 */
void CSCFitXonStripWithGatti::getGattiCalib( float x, int t_disabled ) {

  std::cout << "This should not be called anymore !!!!!!!!!!!!!" << std::endl;
  return;
  double   g0 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x - 2.5)/r ) );
  double   g1 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x - 1.5)/r ) );
  double   g2 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x - 0.5)/r ) );
  double   g3 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x + 0.5)/r ) );
  double   g4 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x + 1.5)/r ) );
  double   g5 = norm * atan( sqrt_k_3 * tanh( k_2 * (-x + 2.5)/r ) );
   
   // Now correct for x-talks: 
  for (int t = 0; t < 3; ++t) {
    q[0][t] = (g2 - g1) * xt_lr0[t] + (g1 - g0) * xt_r[0][t] + (g3 - g2) * xt_l[1][t];
    q[1][t] = (g3 - g2) * xt_lr1[t] + (g2 - g1)  * xt_r[0][t] +(g4 - g3) * xt_l[2][t];
    q[2][t] = (g4 - g3) * xt_lr2[t] + (g3 - g2) * xt_r[1][t] + (g5 - g4) * xt_l[2][t];
  }


}

