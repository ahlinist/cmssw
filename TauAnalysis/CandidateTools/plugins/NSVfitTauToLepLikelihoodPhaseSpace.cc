#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToLepLikelihoodPhaseSpace.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToDaughtersHypothesisBaseT1T2.h"

#include <TMath.h>

#include <limits>

using namespace SVfit_namespace;

template <typename T>
NSVfitTauToLepLikelihoodPhaseSpace<T>::NSVfitTauToLepLikelihoodPhaseSpace(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleLikelihood(cfg)
{
  applySinThetaFactor_ = cfg.exists("applySinThetaFactor") ?
    cfg.getParameter<bool>("applySinThetaFactor") : false;
}

template <typename T>
NSVfitTauToLepLikelihoodPhaseSpace<T>::~NSVfitTauToLepLikelihoodPhaseSpace()
{
// nothing to be done yet...
}

template <typename T>
void NSVfitTauToLepLikelihoodPhaseSpace<T>::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,    pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_nuInvMass,  pluginName_);
}

template <typename T>
double NSVfitTauToLepLikelihoodPhaseSpace<T>::operator()(const NSVfitSingleParticleHypothesis* hypothesis, int polSign) const
{
//--- compute negative log-likelihood for tau lepton decay "leg"
//    to be compatible with three-body decay,
//    assuming constant matrix element,
//    so that energy and angular distribution of decay products is solely determined by phase-space
//
//    NOTE: the parametrization of the three-body decay phase-space is taken from the PDG:
//          K. Nakamura et al. (Particle Data Group), J. Phys. G 37, 075021 (2010);
//          formula 38.20b
//
  const NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesis, T>* hypothesis_T = 
    dynamic_cast<const NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesis, T>*>(hypothesis);
  assert(hypothesis_T != 0);

  double decayAngle = hypothesis_T->decay_angle_rf();
  double nuMass = hypothesis_T->p4invis_rf().mass();
  if ( nuMass < 0. ) nuMass = 0.; // CV: add protection against rounding errors when boosting between laboratory and rest frame
  double visEnFracX = hypothesis_T->visEnFracX();
  double visMass = hypothesis_T->p4vis_rf().mass();
  if ( visMass < 1.e-4 ) visMass = 1.e-4;
  double visMass2 = square(visMass);

  //if ( this->verbosity_ ) {
  //  std::cout << "<NSVfitTauToLepLikelihoodPhaseSpace::operator()>:" << std::endl;
  //  std::cout << " tauLeptonMass2 = " << tauLeptonMass2 << std::endl;
  //  std::cout << " decayAngle = " << decayAngle << std::endl;
  //  std::cout << " nuMass = " << nuMass << std::endl;
  //  std::cout << " visEnFracX = " << visEnFracX << std::endl;
  //  std::cout << " visMass = " << visMass << std::endl;
  //}

  // CV: normalize likelihood function such that 
  //               1
  //       integral  prob dX dMnunu = 1.
  //               0
  double term1 = tauLeptonMass2 - visMass2;
  double term2 = tauLeptonMass2 + visMass2;
  double term3 = tauLeptonMass*visMass;
  double term4 = square(term3);
  double norm_factor = 1./(0.25*(term1*term2 - 4.*term4*TMath::Log(term3) + 4.*term4*TMath::Log(visMass2)));
  //if ( this->verbosity_ ) std::cout << "norm_factor = " << norm_factor << std::endl;
  double prob = 1.;
  if ( nuMass < TMath::Sqrt((1. - visEnFracX)*tauLeptonMass2) ) { // LB: physical solution
    double term5 = tauLeptonMass2 - square(nuMass + visMass);
    double term6 = tauLeptonMass2 - square(nuMass - visMass);
    prob = norm_factor*nuMass*TMath::Sqrt(term5*term6);
  } else {                                                        // LB: unphysical solution
    double nuMass_limit = TMath::Sqrt((1. - visEnFracX)*tauLeptonMass2);
    double term5_limit = tauLeptonMass2 - square(nuMass_limit + visMass);
    double term6_limit = tauLeptonMass2 - square(nuMass_limit - visMass);
    prob = norm_factor*nuMass_limit*TMath::Sqrt(term5_limit*term6_limit);
    prob /= (1. + 1.e+6*square(nuMass - nuMass_limit));
  }
  if ( applySinThetaFactor_ ) prob *= (0.5*TMath::Sin(decayAngle));

  if ( applyVisPtCutCorrection_ ) {
    double probCorr = 1.;
    const double epsilon_regularization = 1.e-1;
    if ( hypothesis_T->p4_fitted().pt() > visPtCutThreshold_ ) {
      double xCut = visPtCutThreshold_/hypothesis_T->p4_fitted().pt();
      probCorr = 1./((1. - xCut) + epsilon_regularization);
    }
    //if ( this->verbosity_ ) std::cout << "probCorr (lep) = " << probCorr << std::endl;
    prob *= probCorr;
  }
  
  //if ( this->verbosity_ ) std::cout << "--> prob = " << prob << std::endl;

  return prob;
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

typedef NSVfitTauToLepLikelihoodPhaseSpace<pat::Electron> NSVfitTauToElecLikelihoodPhaseSpace;
typedef NSVfitTauToLepLikelihoodPhaseSpace<pat::Muon> NSVfitTauToMuLikelihoodPhaseSpace;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToElecLikelihoodPhaseSpace, "NSVfitTauToElecLikelihoodPhaseSpace");
DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToMuLikelihoodPhaseSpace, "NSVfitTauToMuLikelihoodPhaseSpace");
