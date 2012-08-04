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

  gamma_ = TMath::Sqrt(resonance_mass2_*(resonance_mass2_ + resonance_width2_));
  k_ = 2.*TMath::Sqrt(2.)*resonance_mass_*resonance_width_*gamma_/(TMath::Pi()*TMath::Sqrt(resonance_mass2_ + gamma_));

  power_ = cfg.getParameter<double>("power");
}

NSVfitResonanceLikelihoodBreitWigner::~NSVfitResonanceLikelihoodBreitWigner()
{
// nothing to be done yet...
}

double NSVfitResonanceLikelihoodBreitWigner::operator()(const NSVfitResonanceHypothesis* resonance, int polHandedness) const 
{
  if ( this->verbosity_ ) {
    std::cout << "<NSVfitResonanceLikelihoodBreitWigner::operator()>:" << std::endl;
    std::cout << " resonance: mass = " << resonance_mass_ << ", width = " << resonance_width_ << std::endl;
  }

  double mass = resonance->mass();
  if ( this->verbosity_ ) std::cout << " mass = " << mass << std::endl;
  double mass2 = square(mass);

  // normalized relativistic Breit-Wigner distribution
  // taken from http://en.wikipedia.org/wiki/Relativistic_Breit–Wigner_distribution
  double norm_factor = k_;
  double prob = norm_factor/(square(mass2 - resonance_mass2_) + resonance_mass2_*resonance_width2_);
  if ( prob > 0. && power_ != 1. ) prob = TMath::Power(prob, power_); 
  
  //if ( this->verbosity_ ) std::cout << "--> prob = " << prob << std::endl;
  
  return prob;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitResonanceLikelihoodPluginFactory, NSVfitResonanceLikelihoodBreitWigner, "NSVfitResonanceLikelihoodBreitWigner");
