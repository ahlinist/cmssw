#ifndef CSCRecHitB_CSCPeakBinOfStripPulse_h
#define CSCRecHitB_CSCPeakBinOfStripPulse_h

/** \class CSCPeakBinOfStripPulse
 *
 * Concrete algorithmic class used to identify ADC peak pulse height and Tmax
 * on strips in the endcap muon CSCs.
 * This version just finds the SCA bin with maximum content and Tmax.
 *
 * \author Dominique Fortin
 *
 * Modified from existing code in CSCRecHit/src/
 */

class CSCChamberSpecs;
class CSCStripDigi;

class CSCPeakBinOfStripPulse {
  
 public:
  CSCPeakBinOfStripPulse(){};
  ~CSCPeakBinOfStripPulse(){};
  
  /// Find the peak pulseheight from the samples
  bool peakAboveBaseline(const CSCStripDigi& digi, 
                         const CSCChamberSpecs& specs,
                         double& height, int& tpeak, double& sigma) const;


  /// Find the signal after subtracting pedestals (first 2 time bins)  
  bool signalAboveBaseline(const CSCStripDigi& digi,
                           const CSCChamberSpecs& specs,
                           double& height, const int& time) const;
  
  
  /// Find the Strip pulseheight baseline 
  float baseline(const CSCStripDigi& digi) const;
  
};

#endif

