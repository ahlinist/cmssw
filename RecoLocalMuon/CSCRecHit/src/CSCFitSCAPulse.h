#ifndef CSCRecHit_CSCFitSCAPulse_h
#define CSCRecHit_CSCFitSCAPulse_h

/** \class CSCFitSCAPulse
 *
 * Concrete algorithmic class used to identify peak SCA pulse height
 * on strips in the endcap muon CSCs. 
 * (The peak pulse height represents the energy deposited in the strip.) <BR>
 * This version fits the SCA time samples near the peak position 
 * to identify the peak value of the SCA pulse height.
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCFindPulseheightOnStrip.h>
#include <Minuit2/VariableMetricMinimizer.h>
using namespace ROOT::Minuit2;

class CSCPulseHeightFcn;

class CSCFitSCAPulse : public CSCFindPulseheightOnStrip
{
public:
  CSCFitSCAPulse();
  virtual ~CSCFitSCAPulse();

/// Find the peak pulseheight from the SCA samples.
/// The returned bool is success/fail status.
  virtual bool peakAboveBaseline(const CSCStripDigi & input, 
                         const CSCChamberSpecs & specs,
                         double & height, double & sigma) const;
private:
  ModularFunctionMinimizer* theFitter;
  CSCPulseHeightFcn* theOBJfun;
};

#endif

