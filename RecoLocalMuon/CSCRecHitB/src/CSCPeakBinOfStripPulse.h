#ifndef CSCRecHitB_CSCPeakBinOfStripPulse_h
#define CSCRecHitB_CSCPeakBinOfStripPulse_h

/** \class CSCPeakBinOfStripPulse
 *
 * Class used to identify ADC peak pulse height, T_max and T_peak
 * on strips in the endcap muon CSCs.  All of the calibrations are
 * also applied for the strips at this stage: gain, x-talk and noise matrix !
 *
 * \author Dominique Fortin
 *
 */

#include <FWCore/ParameterSet/interface/ParameterSet.h>

class CSCChamberSpecs;
class CSCGains;
class CSCcrosstalk;
class CSCNoiseMatrix;
class CSCStripDigi;
class CSCFindPeakTime;
class CSCCalibrateStrip;

class CSCPeakBinOfStripPulse 
{
  
 public:

  /// configurable parameters
  CSCPeakBinOfStripPulse(const edm::ParameterSet & ps);
  ~CSCPeakBinOfStripPulse();

  /// This finds the strip seed for the cluster, that is the strip with maximum deposition
  bool peakAboveBaseline( const CSCStripDigi& digi, double& height, int& tmax, 
                          float& tpeak, double& sigma) const;

  /// Find the signal after subtracting pedestals (first 2 time bins)  
  bool signalAboveBaseline( const CSCStripDigi& digi, double& height, const int& time) const;
    
  /// Find the Strip pulseheight baseline 
  float baseline( const CSCStripDigi& digi ) const;

  void setCalibration( const CSCGains* gains,
                       const CSCcrosstalk* xtalk,
                       const CSCNoiseMatrix* noise ) {
    gains_ = gains;
    xtalk_ = xtalk;
    noise_ = noise;
  }

 private:

  /*
   * Cache calibrations for current event
   */
  const CSCGains* gains_;
  const CSCcrosstalk* xtalk_;
  const CSCNoiseMatrix* noise_;
 
  float globalGainAvg;

  CSCFindPeakTime* peakTimeFinder_;
  CSCCalibrateStrip* calibrateStrip_;

  int ClusterSize;
  int isData;
};

#endif

