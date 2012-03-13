#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToLepLikelihoodMatrixElement.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToDaughtersHypothesisBaseT1T2.h"

#include <TMath.h>

#include <limits>

using namespace SVfit_namespace;

template <typename T>
NSVfitTauToLepLikelihoodMatrixElement<T>::NSVfitTauToLepLikelihoodMatrixElement(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleLikelihood(cfg)
{
  applySinThetaFactor_ = cfg.exists("applySinThetaFactor") ?
    cfg.getParameter<bool>("applySinThetaFactor") : false;
}

template <typename T>
NSVfitTauToLepLikelihoodMatrixElement<T>::~NSVfitTauToLepLikelihoodMatrixElement()
{
// nothing to be done yet...
}

template <typename T>
void NSVfitTauToLepLikelihoodMatrixElement<T>::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,    pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_nuInvMass,  pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_pol,        pluginName_);
}

template <typename T>
double NSVfitTauToLepLikelihoodMatrixElement<T>::operator()(const NSVfitSingleParticleHypothesis* hypothesis) const
{
//--- compute negative log-likelihood for tau lepton decay 
//    tau- --> e- nu nu (tau- --> mu- nu nu)
//    to be compatible with matrix element of V-A electroweak decay
//   (ignoring tau lepton polarization effects)
//
//    NOTE: The formulas taken from the paper
//           "Tau polarization and its correlations as a probe of new physics",
//           B.K. Bullock, K. Hagiwara and A.D. Martin,
//           Nucl. Phys. B395 (1993) 499.
//
  const NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesis, T>* hypothesis_T = 
    dynamic_cast<const NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesis, T>*>(hypothesis);
  assert(hypothesis_T != 0);

  if ( this->verbosity_ ) std::cout << "<NSVfitTauToLepLikelihoodMatrixElement::operator()>:" << std::endl;
  
  double decayAngle = hypothesis_T->decay_angle_rf();
  double nuMass = hypothesis_T->p4invis_rf().mass();
  if ( nuMass < 0. ) nuMass = 0.; // CV: add protection against rounding errors when boosting between laboratory and rest frame
  double nuMass2 = square(nuMass);
  double visEnFracX = hypothesis_T->visEnFracX();

  if ( this->verbosity_ ) {
    std::cout << " decayAngle = " << decayAngle << std::endl;
    std::cout << " nuMass = " << nuMass << std::endl;
    std::cout << " visEnFracX = " << visEnFracX << std::endl;
  }

  // LB: normalize likelihood function such that 
  //               1
  //       integral  prob dX dMnunu = 1.
  //               0
  double prob = 1.;
  if ( nuMass < TMath::Sqrt((1. - visEnFracX)*tauLeptonMass2) ) { // LB: physical solution
    prob = (13./square(tauLeptonMass2))*cube(tauLeptonMass2 - nuMass2)*(tauLeptonMass2 + 2.*nuMass2)*nuMass;
  } else {                                                        // LB: unphysical solution
    prob = 1e-6;
  }
  if ( applySinThetaFactor_ ) prob *= (0.5*TMath::Sin(decayAngle));
  
  if ( applyVisPtCutCorrection_ ) {
    double probCorr = 1.;
    if ( hypothesis_T->p4_fitted().pt() > visPtCutThreshold_ ) {
      double xCut = visPtCutThreshold_/hypothesis_T->p4_fitted().pt();
      probCorr = 1./(3. - 5.*xCut + 3.*cube(xCut) - fourth(xCut));
    } else {
      probCorr = 1e-6;
    }
    if ( this->verbosity_ ) std::cout << "probCorr (lep) = " << probCorr << std::endl;
    prob *= probCorr;
  }

  double nll = 0.;
  if ( prob > 0. ) {
    nll = -TMath::Log(prob);
  } else {
    nll = std::numeric_limits<float>::max();
  }
  
  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return nll;
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

typedef NSVfitTauToLepLikelihoodMatrixElement<pat::Electron> NSVfitTauToElecLikelihoodMatrixElement;
typedef NSVfitTauToLepLikelihoodMatrixElement<pat::Muon> NSVfitTauToMuLikelihoodMatrixElement;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToElecLikelihoodMatrixElement, "NSVfitTauToElecLikelihoodMatrixElement");
DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToMuLikelihoodMatrixElement, "NSVfitTauToMuLikelihoodMatrixElement");

