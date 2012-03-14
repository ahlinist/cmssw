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

  if ( this->verbosity_ ) std::cout << "<NSVfitTauToLepLikelihoodPhaseSpace::operator()>:" << std::endl;

  double decayAngle = hypothesis_T->decay_angle_rf();
  double nuMass = hypothesis_T->p4invis_rf().mass();
  if ( nuMass < 0. ) nuMass = 0.; // CV: add protection against rounding errors when boosting between laboratory and rest frame
  double visMass = hypothesis_T->p4vis_rf().mass();
  double visMass2 = square(visMass);

  if ( this->verbosity_ ) {
    std::cout << " tauLeptonMass2 = " << tauLeptonMass2 << std::endl;
    std::cout << " decayAngle = " << decayAngle << std::endl;
    std::cout << " nuMass = " << nuMass << std::endl;
    std::cout << " visMass = " << visMass << std::endl;
    std::cout << " square(nuMass + visMass) = " << square(nuMass + visMass) << std::endl;
    std::cout << " square(nuMass - visMass) = " << square(nuMass - visMass) << std::endl;
    std::cout << " invisible rest frame p4: " << hypothesis_T->p4invis_rf() << std::endl;
  }

  // CV: normalize likelihood function such that 
  //               1
  //       integral  prob dX dMnunu = 1.
  //               0
  //
  double term1 = tauLeptonMass2 - visMass2;
  double term2 = tauLeptonMass2 + visMass2;
  double term3 = tauLeptonMass*visMass;
  double term4 = square(term3);
  double norm_factor = 1./(0.25*(term1*term2 - 4.*term4*TMath::Log(term3) + 4.*term4*TMath::Log(visMass2)));
  //std::cout << "norm_factor = " << norm_factor << std::endl;
  double prob = norm_factor*nuMass*TMath::Sqrt((tauLeptonMass2 - square(nuMass + visMass))*(tauLeptonMass2 - square(nuMass - visMass)));
  if ( applySinThetaFactor_ ) prob *= (0.5*TMath::Sin(decayAngle));

  if ( applyVisPtCutCorrection_ ) {
    double probCorr = 1.;
    const double epsilon_regularization = 1.e-1;
    if ( hypothesis_T->p4_fitted().pt() > visPtCutThreshold_ ) {
      double xCut = visPtCutThreshold_/hypothesis_T->p4_fitted().pt();
      probCorr = 1./((1. - xCut) + epsilon_regularization);
    } else {
      const double penalty_factor = 1.e-3;
      probCorr = penalty_factor/((visPtCutThreshold_ - hypothesis_T->p4_fitted().pt()) + penalty_factor*epsilon_regularization);
    }
    if ( this->verbosity_ ) std::cout << "probCorr (lep) = " << probCorr << std::endl;
    prob *= probCorr;
  }
  
  double nll = 0.;
  if ( prob > 0. ) {
    nll = -TMath::Log(prob);
  } else {
    if ( prob < 0. )
      edm::LogWarning ("NSVfitTauToLepLikelihoodPhaseSpace::operator()")
	<< " Unphysical solution: prob = " << prob << " --> returning very large positive number !!";
    nll = std::numeric_limits<float>::max();
  }

  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return nll;
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

typedef NSVfitTauToLepLikelihoodPhaseSpace<pat::Electron> NSVfitTauToElecLikelihoodPhaseSpace;
typedef NSVfitTauToLepLikelihoodPhaseSpace<pat::Muon> NSVfitTauToMuLikelihoodPhaseSpace;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToElecLikelihoodPhaseSpace, "NSVfitTauToElecLikelihoodPhaseSpace");
DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToMuLikelihoodPhaseSpace, "NSVfitTauToMuLikelihoodPhaseSpace");
