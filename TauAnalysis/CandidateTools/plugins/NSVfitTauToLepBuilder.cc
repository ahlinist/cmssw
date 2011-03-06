#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToLepBuilder.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToLepHypothesis.h"

using namespace SVfit_namespace;

template <typename T>
NSVfitTauToLepBuilder<T>::NSVfitTauToLepBuilder(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleBuilderBase(cfg),
    algorithm_(0)
{}

template <typename T>
NSVfitTauToLepBuilder<T>::~NSVfitTauToLepBuilder()
{
// nothing to be done yet...
}

template <typename T>
void NSVfitTauToLepBuilder<T>::beginJob(NSVfitAlgorithmBase* algorithm)
{
  idxFitParameter_visEnFracX_ = getFitParameterIdx(algorithm, prodParticleLabel_, kTau_visEnFracX);
  idxFitParameter_phi_lab_    = getFitParameterIdx(algorithm, prodParticleLabel_, kTau_phi_lab);
  idxFitParameter_nuInvMass_  = getFitParameterIdx(algorithm, prodParticleLabel_, kTau_nuInvMass);
  algorithm_ = algorithm;
}

template <typename T>
NSVfitSingleParticleHypothesisBase* NSVfitTauToLepBuilder<T>::build(const inputParticleMap& inputParticles) const
{
  inputParticleMap::const_iterator particlePtr = inputParticles.find(prodParticleLabel_);
  assert(particlePtr != inputParticles.end());

  NSVfitTauToLepHypothesis<T>* hypothesis = new NSVfitTauToLepHypothesis<T>(particlePtr->second, prodParticleLabel_, barcodeCounter_);
  ++barcodeCounter_;

  const T* lepPtr = dynamic_cast<const T*>(particlePtr->second.get());
  assert(lepPtr);

  hypothesis->tracks_     = trackExtractor_(*lepPtr);

  hypothesis->p3Vis_unit_ = lepPtr->p4().Vect().Unit();
  hypothesis->visMass_    = lepPtr->mass();

  NSVfitAlgorithmBase::fitParameterType* fitParameter = algorithm_->getFitParameter(prodParticleLabel_, kTau_nuInvMass);
  assert(fitParameter);
  fitParameter->upperLimit_ = SVfit_namespace::tauLeptonMass - hypothesis->visMass_;

  return hypothesis;
}

template <typename T>
void NSVfitTauToLepBuilder<T>::applyFitParameter(NSVfitSingleParticleHypothesisBase* hypothesis, double* param) const
{
  NSVfitTauToLepHypothesis<T>* hypothesis_T = dynamic_cast<NSVfitTauToLepHypothesis<T>*>(hypothesis);
  assert(hypothesis_T);

  double visEnFracX = param[idxFitParameter_visEnFracX_];
  double phi_lab    = param[idxFitParameter_phi_lab_];
  double pVis_lab   = hypothesis_T->p4().P();
  double enVis_lab  = hypothesis_T->p4().energy();
  double visMass    = hypothesis_T->visMass();
  double nuInvMass  = param[idxFitParameter_nuInvMass_];

  const reco::Candidate::Vector& p3Vis_unit = hypothesis_T->p3Vis_unit();

//--- compute momentum of visible decay products in tau lepton rest frame
  double pVis_rf = pVisRestFrame(visMass, nuInvMass);
  double enVis_rf = energyFromMomentum(pVis_rf, tauLeptonMass);

//--- decay angle in tau lepton rest frame as function of X
//    (= energy ratio of visible decay products/tau lepton energy)
  double beta = TMath::Sqrt(1. - square(tauLeptonMass*visEnFracX/enVis_lab));
  double cosGjAngle = (tauLeptonMass*visEnFracX - enVis_rf)/(pVis_rf*beta);
  double gjAngle = TMath::ACos(cosGjAngle);

//--- compute tau lepton decay angle in laboratory frame
  double angleVis_lab = SVfit_namespace::gjAngleToLabFrame(pVis_rf, gjAngle, pVis_lab);

//--- compute tau lepton momentum in laboratory frame
  double pTau_lab = SVfit_namespace::tauMomentumLabFrame(visMass, pVis_rf, gjAngle, pVis_lab);

//--- compute tau lepton direction in laboratory frame
  reco::Candidate::Vector p3Tau = SVfit_namespace::tauDirection(p3Vis_unit, angleVis_lab, phi_lab);

//--- compute tau lepton four-vector in laboratory frame
  reco::Candidate::LorentzVector p4Tau = SVfit_namespace::tauP4(p3Tau.Unit(), pTau_lab);

  hypothesis_T->p4_fitted_      = p4Tau;
  hypothesis_T->dp4_            = (p4Tau - hypothesis_T->p4_);

  hypothesis_T->p4invis_rf_     = boostToCOM(p4Tau, hypothesis_T->dp4_);
  hypothesis_T->p4vis_rf_       = boostToCOM(p4Tau, hypothesis_T->p4());

  if ( verbosity_ ) {
    std::cout << "<NSVfitTauToLepBuilder::applyFitParameter>:" << std::endl;
    std::cout << " visEnFracX = " << param[idxFitParameter_visEnFracX_] << std::endl;
    std::cout << " phi_lab = " << param[idxFitParameter_phi_lab_] << std::endl;
    std::cout << " enVis_lab = " << enVis_lab << std::endl;
    std::cout << " visMass = " << visMass << std::endl;
    std::cout << " nuInvMass = " << param[idxFitParameter_nuInvMass_] << std::endl;
    std::cout << " gjAngle = " << gjAngle << std::endl;
    std::cout << " angleVis_lab = " << angleVis_lab << std::endl;
    std::cout << " pTau_lab = " << pTau_lab << std::endl;
    std::cout << "p4Vis: E = " << hypothesis_T->p4_.energy() << "," 
	      << " px = " << hypothesis_T->p4_.px() << ", py = " << hypothesis_T->p4_.py() << "," 
	      << " pz = " << hypothesis_T->p4_.pz() << std::endl;
    std::cout << "p4Tau: E = " << p4Tau.energy() << "," 
	      << " px = " << p4Tau.px() << ", py = " << p4Tau.py() << "," 
	      << " pz = " << p4Tau.pz() << std::endl;
  }
  
  hypothesis_T->visEnFracX_     = visEnFracX;
  hypothesis_T->decay_angle_rf_ = gjAngle;
}

template <typename T>
void NSVfitTauToLepBuilder<T>::print(std::ostream& stream) const
{
  stream << "<NSVfitTauToLepBuilder::print>:" << std::endl;
  stream << " pluginName = " << pluginName_ << std::endl;
  stream << " pluginType = " << pluginType_ << std::endl;
  stream << " prodParticleLabel = " << prodParticleLabel_ << std::endl;
  stream << " idxFitParameter_visEnFracX = " << idxFitParameter_visEnFracX_ << std::endl;
  stream << " idxFitParameter_phi_lab = " << idxFitParameter_phi_lab_ << std::endl;
  stream << " idxFitParameter_nuInvMass = " << idxFitParameter_nuInvMass_ << std::endl;
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

typedef NSVfitTauToLepBuilder<pat::Electron> NSVfitTauToElecBuilder;
typedef NSVfitTauToLepBuilder<pat::Muon> NSVfitTauToMuBuilder;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleBuilderPluginFactory, NSVfitTauToElecBuilder, "NSVfitTauToElecBuilder");
DEFINE_EDM_PLUGIN(NSVfitSingleParticleBuilderPluginFactory, NSVfitTauToMuBuilder, "NSVfitTauToMuBuilder");

