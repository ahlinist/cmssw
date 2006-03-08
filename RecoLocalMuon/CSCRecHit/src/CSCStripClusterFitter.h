#ifndef CSCRecHit_CSCStripClusterFitter_h
#define CSCRecHit_CSCStripClusterFitter_h

/** \class CSCStripClusterFitter
 *
 * Concrete algorithmic class used to determine a hit position from 
 * a cluster of hit strips in the endcap muon CSCs.
 * This version estimates the cluster position by fitting the
 * pulseheights on the strips in the cluster to some function.
 * We use the semi-theoretical/semi-empirical 'Gatti Function'
 * form expected for the charge distribution on CSC strips. <BR>
 * (The 'pulseheight' on a strip must be extracted from the SCA time-samples.)
 * In ORCA, this fit has used - in historical order - Minuit, NAG, Gemini, C++ Minuit...
 *
 * \author Rick Wilkinson
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCFindPositionOfStripCluster.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCStripClusterCentroid.h>
#include <Minuit2/VariableMetricMinimizer.h>

using namespace ROOT::Minuit2;

class CSCStripClusterFcn;
class CSCChamberSpecs;

class CSCStripClusterFitter : public CSCFindPositionOfStripCluster
{
public:
  /// ctor allows defaults.
  /// if you don't specify a fitter and chisq function, defaults are assigned.
  CSCStripClusterFitter(ModularFunctionMinimizer* minimizer = 0, CSCStripClusterFcn * function = 0);
  virtual ~CSCStripClusterFitter();

/// Feed specs to function
  virtual void initChamberSpecs(const CSCChamberSpecs &);

/// return 'position' in 'u' and its estimated error in 'sigma'.
  void findPosition(const std::vector<CSCFitData> & data,
                   double& u, double&  sigma, double& chisq, double& prob);

private:
  ModularFunctionMinimizer* pMinimizer;
  CSCStripClusterFcn* pFcn;
  // backup for when the fit chi2 is bad
  CSCStripClusterCentroid theCentroidClusterer;
  float maxChi2; //@@ This value must be configurable
};

#endif
