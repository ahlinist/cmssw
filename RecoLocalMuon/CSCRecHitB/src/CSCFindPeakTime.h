/** This is CSCFindPeakTime
 *
 * \author Dominique Fortin
 *
 * Based on RecoLocalMuon/CSCStandAlone/interface/PulseTime.h  by S. Durkin       <BR>
 *										  <BR>
 * Fast fit adc values to                                     			  <BR>
 *    N*(p0**2/256/exp(-4))*(t-t0)**4*exp(-p0*(t-t0))				  <BR>
 *										  <BR>
 * PulseTime has peaking time fixed to 133 nsec.				  <BR>
 * PulseTime_full lets peaking time vary.					  <BR>
 *										  <BR>
 * Fit the log of data                                                     	  <BR>
 *                      chi2=(log(Data)-log(N)-4*log(t-t0)+p0*(t-t0))**2	  <BR>
 *                                                                                <BR>
 * Becomes least square fit in p0 and log(N); and binary search in t0             <BR>
 * yields N, p0, t0(nsec)				                          <BR>
 *										  <BR>
 * Note: tpeak=4/p0 (nsec) and adc[0] is arbitrarily defined a time of 0.0 nsec.  <BR>
 *
 */

class CSCStripDigi;

class CSCFindPeakTime
{
 public:
  
  CSCFindPeakTime(){}; 
  
  ~CSCFindPeakTime(){}; 
  
  /// Member functions
  float FindPeakTime( const int& tmax, const float* adc ); 
  float ChiSqr()   { return Chi_min; }
  float StartTime(){ return t0; }
  float PeakTime() { return t_peak; }
  float PeakChg()  { return Q_peak; }
  
 private:
  
  float t0, t_peak, Chi_min, Q_peak;
  
};
