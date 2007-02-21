#ifndef CSCRecHit_CSCPeakBinOfSCAPulse_h
#define CSCRecHit_CSCPeakBinOfSCAPulse_h

/** \class CSCPeakBinOfSCAPulse
 *
 * Concrete algorithmic class used to identify peak SCA pulse height
 * on strips in the endcap muon CSCs.
 * (The peak pulse height represents the energy deposited in the strip.) <BR>
 * This version just finds the SCA bin with maximum content.
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCFindPulseheightOnStrip.h>

class CSCPeakBinOfSCAPulse : public CSCFindPulseheightOnStrip {
public:

  CSCPeakBinOfSCAPulse(){};
  virtual ~CSCPeakBinOfSCAPulse(){};

  /// Find the peak pulseheight from the SCA samples
  virtual bool peakAboveBaseline(const CSCStripDigi& input, 
                         const CSCChamberSpecs& specs,
                         double& height, int& tmax,
                         StripHitADCContainer& adcs, double& sigma) const;

};

#endif

