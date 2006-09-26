/* This is CSCFindPeakTime
 *
 * \author Dominique Fortin
 *
 * migrated from PulseTime.h originally written by S. Durkin
 */ 

#include <RecoLocalMuon/CSCRecHitB/src/CSCFindPeakTime.h>

#include<iostream>

#include <vector>


/* FindPeakTime
 *
 */
void CSCFindPeakTime::FindPeakTime( const int& tmax, const float* adc, float& t_peak, std::vector<float>& adcFit ) {
  
  // Initialize parameters in case fit fails
  float t0       = 0.;
  float p0       = 4./133.;
  float N        = adc[1];
  t_peak         = tmax * 50.;
  for ( int i = 0; i < 4; i++ ) adcFit[i] = adc[i];
 

  // If outside physical range, exit
  if ( tmax < 2 || tmax > 7 ) return;

  float tb[4], db[4];
  for ( int time=0; time<4; time++ ){
    tb[time] = (tmax + time -1) * 50.;
    db[time] = adc[time];
  }
  
  float sx2  = 0.;
  float sxy  = 0.;
  int n_fit  = 4;

  if ( tmax == 6 ) n_fit = 3;
  
  float del_t    = 50.;
  float chi_min  = 1.e10;
  float chi_last = 1.e10;
  float tt0      = 0.;

  int i_count = 0;
  
  while ( del_t > 0.05 ) {
    
    float x[4], y[4];
    sx2 = 0.;
    sxy = 0.;
    
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
    if (i_count > 1000) break;
  }

  t_peak = 4.0 / p0;


  // Now fit charge so have tb[1] = peak charge 
  for (int i=0; i < 4; i++) {
    tb[i] = tb[i] + t_peak - tb[1];
    adcFit[i] = N*(tb[i]-t0)*(tb[i]-t0)*(tb[i]-t0)*(tb[i]-t0)*exp(-p0*(tb[i]-t0));
  }
  return;
}


