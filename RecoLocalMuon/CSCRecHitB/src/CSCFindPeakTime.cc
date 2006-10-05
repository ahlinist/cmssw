/* This is CSCFindPeakTime
 *
 * \author Dominique Fortin
 *
 * adapted from PulseTime.h originally written by S. Durkin
 */ 

#include <RecoLocalMuon/CSCRecHitB/src/CSCFindPeakTime.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/Exception.h>

#include <cmath>
#include <iostream>

#include <vector>


/* FindPeakTime
 *
 */
void CSCFindPeakTime::FindPeakTime( const int& tmax, const float* adc, float& t_peak, std::vector<float>& adcsFit ) {
  
  // Initialize parameters in case fit fails
  float t0       = 0.;
  float N        = adc[1];
  t_peak         = 150.;
  float p0       = 4./t_peak;

  // If outside physical range, exit
  if ( tmax < 2 || tmax > 6 ) return;

  float tb[4], y[4];
  for ( int time=0; time<4; time++ ){
    tb[time] = (tmax + time -1) * 50.;
    y[time] = adc[time];
  }

  int n_fit  = 4;
  if ( tmax == 6 ) n_fit = 3;

  float del_tpeak = 100.;
  int i_count = 0;
  float chi_min  = 1.e10;
  float chi_last = 1.e10;

  while ( del_tpeak > 1. ) {

    p0             = 4./t_peak;  
    float del_t    = 100.;
    float tt0      = 0.;
    float chi2     = 0.;

    while ( del_t > 0.05 ) {
    
      float x[4], y[4];
      float sx2 = 0.;
      float sxy = 0.;
    
      for ( int j=0; j < n_fit; j++ ) {
        float t = tb[j];
        x[j] = (t-tt0)*(t-tt0)*(t-tt0)*(t-tt0) * exp( -p0 * (t-tt0) );
        sx2  = sx2 + x[j] * x[j];
        sxy  = sxy + x[j] * y[j];
      }
      float NN = sxy / sx2;
    
      // Compute chi^2
      chi2 = 0.0;
      for (int j=0; j < n_fit; j++) chi2 += (y[j] - NN * x[j]) * (y[j] - NN * x[j]);

      // Test on chi^2 to decide what to do    
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
    }
    // Test on chi^2 to decide what to do
    if ( chi_last > chi2 ) {
      chi_last  = chi2;
      t_peak    = t_peak + del_tpeak;
    } else {
      t_peak   = t_peak - 2. * del_tpeak;
      del_tpeak= del_tpeak / 2.;    
      t_peak   = t_peak + del_tpeak;
      chi_last = 1.0e10;
    }      
    if (i_count > 1000) break;
  }

  if (i_count > 0) std::cout << "Fit for finding peak time failed" << std::endl;

  p0 = 4./t_peak;

  std::cout << "peaking time is " << t_peak << std::endl;

  // Now fit charge so have tb[1] = peak charge 
  for ( int i = -1; i < 2; i++ ) {
    float t = t_peak + i * 50.;
    float q_fitted = N * (t-t0)*(t-t0)*(t-t0)*(t-t0) * exp( -p0 * (t-t0) );
    adcsFit.push_back(q_fitted);
  }
  return;
}


