#include "TauAnalysis/CandidateTools/plugins/NSVfitResonanceLikelihoodBreitWigner.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

using namespace SVfit_namespace;

NSVfitResonanceLikelihoodBreitWigner::NSVfitResonanceLikelihoodBreitWigner(const edm::ParameterSet& cfg)
  : NSVfitResonanceLikelihood(cfg)
{
  resonance_mass_   = cfg.getParameter<double>("mass");
  resonance_mass2_  = square(resonance_mass_);
  resonance_width_  = cfg.getParameter<double>("width");
  resonance_width2_ = square(resonance_width_);

  power_ = cfg.getParameter<double>("power");
}

NSVfitResonanceLikelihoodBreitWigner::~NSVfitResonanceLikelihoodBreitWigner()
{
// nothing to be done yet...
}

double NSVfitResonanceLikelihoodBreitWigner::operator()(const NSVfitResonanceHypothesis* resonance, int polHandedness) const 
{
  if ( this->verbosity_ ) std::cout << "<NSVfitResonanceLikelihoodBreitWigner::operator()>:" << std::endl;

  double mass = resonance->mass();
  if ( this->verbosity_ ) std::cout << " mass = " << mass << std::endl;
  double mass2 = square(mass);

  double gamma = TMath::Sqrt(mass2*(mass2 + resonance_width2_));
  const double const_factor = 2.*TMath::Sqrt(2.)*resonance_width_/TMath::Pi();
  double norm_factor = const_factor*mass*gamma/TMath::Sqrt(mass2 + gamma);
  double prob = norm_factor/(square(mass2 - resonance_mass2_) + mass2*resonance_width2_);

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

DEFINE_EDM_PLUGIN(NSVfitResonanceLikelihoodPluginFactory, NSVfitResonanceLikelihoodBreitWigner, "NSVfitResonanceLikelihoodBreitWigner");
