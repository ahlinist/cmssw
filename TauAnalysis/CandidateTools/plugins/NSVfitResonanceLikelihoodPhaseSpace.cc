#include "TauAnalysis/CandidateTools/plugins/NSVfitResonanceLikelihoodPhaseSpace.h"

#include <TMath.h>

NSVfitResonanceLikelihoodPhaseSpace::NSVfitResonanceLikelihoodPhaseSpace(const edm::ParameterSet& cfg)
  : NSVfitResonanceLikelihood(cfg)
{
  applySinThetaFactor_ = cfg.exists("applySinThetaFactor") ?
    cfg.getParameter<bool>("applySinThetaFactor") : false;

  power_ = cfg.getParameter<double>("power");
}

NSVfitResonanceLikelihoodPhaseSpace::~NSVfitResonanceLikelihoodPhaseSpace()
{
// nothing to be done yet...
}

double NSVfitResonanceLikelihoodPhaseSpace::operator()(const NSVfitResonanceHypothesis* resonance, int polHandedness) const 
{
  if ( this->verbosity_ ) std::cout << "<NSVfitResonanceLikelihoodPhaseSpace::operator()>:" << std::endl;

  double prodAngle_rf = resonance->prod_angle_rf();
  if ( this->verbosity_ ) std::cout << " prodAngle_rf = " << prodAngle_rf << std::endl;

  double prob = 1.;
  if ( applySinThetaFactor_ ) prob *= (0.5*TMath::Sin(prodAngle_rf)); // phase-space factor 
				                                      // (to be used only in "fit", **not** in integration mode)

  double nll = 0.;
  if ( prob > 0. ) {
    nll = -power_*TMath::Log(prob);
  } else {
    nll = std::numeric_limits<float>::max();
  }
  
  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return nll;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitResonanceLikelihoodPluginFactory, NSVfitResonanceLikelihoodPhaseSpace, "NSVfitResonanceLikelihoodPhaseSpace");
