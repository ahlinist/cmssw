#ifndef CSCRecHit_CSCTotalSCACounts_h
#define CSCRecHit_CSCTotalSCACounts_h

/** \class CSCTotalSCACounts
 *
 * Concrete algorithmic class used to identify peak SCA pulse height
 * on strips in the endcap muon CSCs. 
 * (The peak pulse height represents the energy deposited in the strip.) <BR>
 * This version was a simulation-only cheat in ORCA 
 * and used the total SCA pulse height so not very realistic.
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCFindPulseheightOnStrip.h>

class CSCTotalSCACounts: public CSCFindPulseheightOnStrip {

public:
  CSCTotalSCACounts() {};
  virtual ~CSCTotalSCACounts() {};

  /// Find the peak pulseheight from the SCA samples
  /// In ORCA this used the true total charge on strips, not SCA information,
  /// but this is not the way the SCA is designed to work so
  /// not all that useful. In CMSSW we don't have that total charge value anyway.
  virtual bool peakAboveBaseline(const CSCStripDigi & input, 
                         const CSCChamberSpecs & specs,
                         double & height, double & sigma) const;
};

#endif

