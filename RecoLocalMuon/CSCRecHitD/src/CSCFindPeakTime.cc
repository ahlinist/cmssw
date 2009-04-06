// This is CSCFindPeakTime

#include <RecoLocalMuon/CSCRecHitD/src/CSCFindPeakTime.h>
//#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <cmath>
//#include <iostream>

CSCFindPeakTime::CSCFindPeakTime( const edm::ParameterSet& ps ): 
  useAverageTime(false), useParabolaFit(false), useFourPoleFit(false) {

  useAverageTime = ps.getParameter<bool>("UseAverageTime");
  useParabolaFit = ps.getParameter<bool>("UseParabolaFit");
  useFourPoleFit = ps.getParameter<bool>("UseFourPoleFit");

}

float CSCFindPeakTime::peakTime( int tmax, const float* adc, float t_peak){
  if ( useAverageTime ) {
    return averageTime( tmax, adc );
  }
  else if ( useParabolaFit ) {
    return parabolaFitTime( tmax, adc );
  }
  else if ( useFourPoleFit ) {
     return fourPoleFitTime( tmax, adc, t_peak);
  }
  else {
  // return something, anyway.. may as well be average
    return averageTime( tmax, adc );
  }
}

float CSCFindPeakTime::averageTime( int tmax, const float* adc ) {
   float sum  = 0.;
   float sumt = 0.;
   for (size_t i=0; i<4; ++i){
     sum  += adc[i];
     sumt += adc[i] * ( tmax - 1 + i );

   }
   return sumt/sum * 50.; //@@ in ns. May be some bin width offset things to handle here?
}

float CSCFindPeakTime::parabolaFitTime( int tmax, const float* adc ) {
  // 3-point parabolic fit, courtesy Andy Kubik
 
   float timing = tmax; // @@ Reasonable default?
 
   float x1 = tmax-1;
   float x2 = tmax;
   float x3 = tmax+1;
   // By construction, input array adc is for bins tmax-1 to tmax+2
   float y1 = adc[0];
   float y2 = adc[1];
   float y3 = adc[2];
   float top = (x2*x2-x1*x1)*(y3-y1) - (x3*x3-x1*x1)*(y2-y1);
   float bottom = (x1-x3)*(y2-y1) - (y3-y1)*(x1-x2);
   if (bottom != 0) timing = top/(2*bottom);
   return timing * 50.; //@@ in ns. May be some bin width offset things to handle here?
}

float CSCFindPeakTime::fourPoleFitTime( int tmax, const float* adc, float t_peak ) {

  // Input is 
  // tmax   = bin# 0-7 containing max SCA pulse height  
  // adc    = 4-dim array containing SCA pulse heights in bins tmax-1 to tmax+2
  // t_peak = input estimate for SCA peak time

  // Returned value is improved (we hope) estimate for SCA peak time

  // Algorithm is to fit four-pole function for start time of SCA pulse, t0
  // (The SCA peak is assumed to be 133 ns from t0.)

  // Initialize parameters to sensible (?) values

  float fpNorm   = adc[1]; // this is tmax bin
  float t0       = 0.;
  float t0peak   = 133.;   // this is offset of peak from start time t0
  float p0       = 4./t0peak;

  // Require that tmax is in range 2-6 of bins the eight SCA time bins 0-7
  // (Bins 0, 1 used for dynamic ped)

  if ( tmax < 2 || tmax > 6 ) return t_peak; //@@ Just return the input value

  // Set up time bins to match adc[4] input

  float tb[4];
  for ( int time=0; time<4; ++time ){
    tb[time] = (tmax + time -1) * 50.;
  }

  // How many time bins are we fitting?

  int n_fit  = 4;
  if ( tmax == 6 ) n_fit = 3;

  float chi_min  = 1.e10;
  float chi_last = 1.e10;
  float tt0      = 0.;
  float chi2     = 0.;
  float del_t    = 100.;

  float x[4];
  float sx2 = 0.;
  float sxy = 0.;
  float fN = 0.;

  while ( del_t > 1. ) {
    sx2 = 0.;
    sxy = 0.;
        
    for ( int j=0; j < n_fit; ++j ) {
      x[j] = (tb[j] - tt0) * (tb[j] - tt0) * (tb[j] - tt0) * (tb[j] - tt0) * exp( -p0 * (tb[j] - tt0) );
      sx2  += x[j] * x[j];
      sxy  += x[j] * adc[j];
    }
    fN = sxy / sx2; // least squares fit over time bins i to adc[i] = fN * fourPoleFunction[i]
    
    // Compute chi^2
    chi2 = 0.0;
    for (int j=0; j < n_fit; ++j) chi2 += (adc[j] - fN * x[j]) * (adc[j] - fN * x[j]);

    // Test on chi^2 to decide what to do    
    if ( chi_last > chi2 ) {
      if (chi2 < chi_min ){
        t0      = tt0;
        fpNorm  = fN;
      }
      chi_last  = chi2;
      tt0       = tt0 + del_t;
    } else {
      tt0      = tt0 - 2. * del_t;
      del_t    = del_t / 2.;
      tt0      = tt0 + del_t;
      chi_last = 1.0e10;
    }
  }

  return t0 + t0peak;
}



void CSCFindPeakTime::fourPoleFitCharge( int tmax, const float* adc, const float& t_zero, const float& t_peak, std::vector<float>& adcsFit ) {

  //@@ This code can certainly be replaced by fourPoleFitTime above, but I haven't time to do that now (Tim).

  float p0  = 4./t_peak;
  float tt0 = t_zero;
  int n_fit = 4;
  if ( tmax == 6 ) n_fit=3;
  
  float tb[4], y[4];
  for ( int t = 0; t < 4; ++t ){
    tb[t] = (tmax + t - 1) * 50.;
    y[t] = adc[t];
  }

  // Find the normalization factor for the function
  float x[4];    
  float sx2 = 0.;
  float sxy = 0.;
  for ( int j=0; j < n_fit; ++j ) {
    float t = tb[j];
    x[j] = (t-tt0)*(t-tt0)*(t-tt0)*(t-tt0) * exp( -p0 * (t-tt0) );
    sx2  = sx2 + x[j] * x[j];
    sxy  = sxy + x[j] * y[j];
  }
  float N = sxy / sx2;
    

  // Now compute charge for a given t  --> only need charges at: t_peak-50, t_peak and t_peak+50
  for ( int i = 0; i < 3; ++i ) {
    float t = t_peak + (i - 1) * 50.;
    float q_fitted = N * (t-tt0)*(t-tt0)*(t-tt0)*(t-tt0) * exp( -p0 * (t-tt0) );
    adcsFit.push_back(q_fitted);
  }
  return;
}


