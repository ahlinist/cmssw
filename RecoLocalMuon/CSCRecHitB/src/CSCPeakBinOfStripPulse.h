#ifndef CSCRecHitB_CSCPeakBinOfStripPulse_h
#define CSCRecHitB_CSCPeakBinOfStripPulse_h

/** \class CSCPeakBinOfStripPulse
 *
 *
 * Class used to identify ADC peak pulse height, T_max and T_peak
 * on strips in the endcap muon CSCs.
 *
 * \author Dominique Fortin
 *
 */

class CSCChamberSpecs;
class CSCStripDigi;
class CSCFindPeakTime;

class CSCPeakBinOfStripPulse {
  
 public:
  CSCPeakBinOfStripPulse();
  ~CSCPeakBinOfStripPulse();


  /// This finds the strip seed for the cluster, that is the strip with maximum deposition
  bool peakAboveBaseline( const CSCStripDigi& digi, double& height, int& tmax, 
                          float& tpeak, double& sigma) const;


  /// Find the signal after subtracting pedestals (first 2 time bins)  
  bool signalAboveBaseline( const CSCStripDigi& digi, double& height, const int& time) const;
  
  
  /// Find the Strip pulseheight baseline 
  float baseline( const CSCStripDigi& digi ) const;


 private:
 
  CSCFindPeakTime* peakTimeFinder_;
  
};

#endif

