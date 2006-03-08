#ifndef CSCRecHit_CSCFixedBinOfSCAPulse_h
#define CSCRecHit_CSCFixedBinOfSCAPulse_h

/** \class CSCFixedBinOfSCAPulse
 *
 * Concrete algorithmic class used to identify peak SCA pulse height
 * on strips in the endcap muon CSCs.
 * (The peak pulse height represents the energy deposited in the strip.) <BR>
 * This version assumes the peak has been set to fall in the 5th SCA sample.
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCFindPulseheightOnStrip.h>

class CSCFixedBinOfSCAPulse : public CSCFindPulseheightOnStrip {
public:
  CSCFixedBinOfSCAPulse(){};
  virtual ~CSCFixedBinOfSCAPulse(){};

  /// Find the peak pulseheight from the SCA samples
  virtual bool peakAboveBaseline(const CSCStripDigi & input, 
                         const CSCChamberSpecs & specs,
                         double & height, double & sigma) const;
  
};

#endif

