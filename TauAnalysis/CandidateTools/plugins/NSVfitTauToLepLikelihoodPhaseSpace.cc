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
// nothing to be done yet...
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
double NSVfitTauToLepLikelihoodPhaseSpace<T>::operator()(const NSVfitSingleParticleHypothesis* hypothesis) const
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
  double sinDecayAngle = TMath::Sin(decayAngle);
  double nuMass = hypothesis_T->p4invis_rf().mass();
  if ( nuMass < 0. ) nuMass = 0.; // CV: add protection against rounding errors when boosting between laboratory and rest frame
  double visMass = hypothesis_T->p4vis_rf().mass();

  if ( this->verbosity_ ) {
    std::cout << " tauLeptonMass2 = " << tauLeptonMass2 << std::endl;
    std::cout << " decayAngle = " << decayAngle << std::endl;
    std::cout << " sinDecayAngle = " << sinDecayAngle << std::endl;
    std::cout << " nuMass = " << nuMass << std::endl;
    std::cout << " visMass = " << visMass << std::endl;
    std::cout << " square(nuMass + visMass) = " << square(nuMass + visMass) << std::endl;
    std::cout << " square(nuMass - visMass) = " << square(nuMass - visMass) << std::endl;
    std::cout << " invisible rest frame p4: " << hypothesis_T->p4invis_rf() << std::endl;
  }

  const double const_factor = (1./fifth(2.*TMath::Pi()))*(1./(64.*tauLeptonMass2));
  double prob = const_factor*nuMass
               *TMath::Sqrt((tauLeptonMass2 - square(nuMass + visMass))*(tauLeptonMass2 - square(nuMass - visMass)))
            /* *sinDecayAngle */;

  if ( applyVisPtCutCorrection_ ) {
    double probCorr = evaluateVisPtCutCorrection(hypothesis);
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
