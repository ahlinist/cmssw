#ifndef CSCRecHit_CSCFindPulseheightOnStrip_h
#define CSCRecHit_CSCFindPulseheightOnStrip_h

/** \class CSCFindPulseheightOnStrip
 *
 * Abstract base for algorithmic classes used to identify peak SCA pulse height
 * on strips in the endcap muon CSCs.
 * The peak pulse height represents the energy deposited in the strip.
 */

class CSCChamberSpecs;
class CSCStripDigi;

class CSCFindPulseheightOnStrip {
public:
  CSCFindPulseheightOnStrip() {};
  virtual ~CSCFindPulseheightOnStrip() {};

/// Find the peak pulseheight from the SCA samples.
/// The returned bool allows a success/fail status.
  virtual bool peakAboveBaseline(const CSCStripDigi& input, 
                         const CSCChamberSpecs& specs,
                         double& height, double& sigma) const = 0;
 
/// Find the SCA pulseheight baseline - with a default implementation
  virtual float baseline(const CSCStripDigi& digi) const;

};

#endif

