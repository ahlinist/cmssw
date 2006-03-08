#ifndef CSCRecHit_CSCStripClusterFcn_h
#define CSCRecHit_CSCStripClusterFcn_h

/** \class CSCStripClusterFcn
 *
 * Specify functional form for fit to cluster of strips in endcap muon CSCs.
 * The functional form is a Gatti distribution.
 * This fit is performed with ROOT Minuit2.
 *
 */

#include "Minuit2/FCNBase.h"
#include <RecoLocalMuon/CSCRecHit/src/CSCFitData.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCGattiFunction.h>

#include <vector>

using namespace ROOT::Minuit2;

class CSCChamberSpecs;

class CSCStripClusterFcn : public FCNBase {
public:
  void initChamberSpecs(const CSCChamberSpecs & chamberSpecs);

  /// Cache event data
  void setEventData(const std::vector<CSCFitData> & data) {theData = data;};

  /**
   * Provide the chi-squared function for the given data,
   * based on the input parameters {x, Qtotal}
   * Note that it is the user's responsibility to keep the Gatti function
   * up to date with the current chamber specs
   */
  virtual double operator()(const std::vector<double>&) const;

  virtual double Up() const {return 1.;}

private:
  // event data
  std::vector<CSCFitData> theData;

  /// The Gatti function for the charge distribution
  CSCGattiFunction fitFunction_;

  /// noise constants for the detector
  double theNoiseTerm, theCalibrationError;
};

#endif

