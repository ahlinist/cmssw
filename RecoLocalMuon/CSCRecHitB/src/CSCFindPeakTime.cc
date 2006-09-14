/* This is CSCFindPeakTime
 *
 * \author Dominique Fortin
 *
 * migrated from PulseTime.h originally written by S. Durkin
 */ 

#include <RecoLocalMuon/CSCRecHitB/src/CSCFindPeakTime.h>
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"

#include<iostream>


/* FindPeakTime
 *
 */
float CSCFindPeakTime::FindPeakTime( const int& tmax, const float* adc ) {
  
  // initialize parameters
  t0       = -99;
  float p0 =  4./133.;
  float N  = -99.;
  t_peak   = 133.*4;
  Q_peak   = -99.;
  
  // If outside physical range, exit
  if ( tmax < 2 || tmax > 7 ) return t_peak;

  float tb[4], db[4];
  for ( int time=0; time<4; time++ ){
    tb[time] = (tmax + time -1) * 50.;
    db[time] = adc[time];
  }
  
  float sx2  = 0.;
  float sxy  = 0.;

  int n_fit  = 4;
  if ( tmax == 6 ) n_fit=3;
  
  float del_t    = 50.;
  float chi_min  = 1.e10;
  float chi_last = 1.e10;
  float tt0      = 0.;

  int i_count = 0;
  
  while ( del_t > 0.05 ) {
    
    float x[4],y[4];
    sx2 = 0.0;
    sxy = 0.0;
    
    for ( int j=0; j < n_fit; j++ ) {
      float t = tb[j];
      x[j] = (t-tt0)*(t-tt0)*(t-tt0)*(t-tt0) * exp( -p0 * (t-tt0) );
      y[j] = db[j];
      sx2  = sx2 + x[j] * x[j];
      sxy  = sxy + x[j] * y[j];
    }
    float NN   = sxy / sx2;
    
    // Compute chi^2
    float chi2 = 0.0;
    for (int j=0; j < n_fit; j++) chi2 += (y[j] - NN * x[j]) * (y[j] - NN * x[j]);

    // Test on chi^2 do decide what to do    
    if ( chi_last > chi2 ) {
      if (chi2 < chi_min ){
        t0      = tt0;
        Chi_min = chi2;
        N       = NN;
      }
      chi_last  = chi2;
      tt0       = tt0 + del_t;
    } else {
      tt0      = tt0 - 2. * del_t;
      del_t    = del_t / 2.;
      tt0      = tt0 + del_t;
      chi_last = 1.0e10;
    }

    i_count++;
    if (i_count > 10000) break;
  }

  t_peak = 4.0 / p0;
  Q_peak = N * 256. * exp(-4.0)/(p0*p0*p0*p0); 
  
  return t_peak;
}
