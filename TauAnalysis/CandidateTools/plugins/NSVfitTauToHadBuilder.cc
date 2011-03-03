#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToHadBuilder.h"

#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToHadHypothesis.h"

using namespace SVfit_namespace;

NSVfitTauToHadBuilder::NSVfitTauToHadBuilder(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleBuilderBase(cfg),
    algorithm_(0)
{}

NSVfitTauToHadBuilder::~NSVfitTauToHadBuilder()
{
// nothing to be done yet...
}

void NSVfitTauToHadBuilder::beginJob(NSVfitAlgorithmBase* algorithm)
{
  idxFitParameter_visEnFracX_ = getFitParameterIdx(algorithm, prodParticleLabel_, kTau_visEnFracX);
  idxFitParameter_phi_lab_    = getFitParameterIdx(algorithm, prodParticleLabel_, kTau_phi_lab);
}

NSVfitSingleParticleHypothesisBase* NSVfitTauToHadBuilder::build(const inputParticleMap& inputParticles) const
{
  inputParticleMap::const_iterator particlePtr = inputParticles.find(prodParticleLabel_);
  assert(particlePtr != inputParticles.end());

  std::string hypothesisName = std::string("NSVfitTauToHadHypothesis").append(":").append(prodParticleLabel_);
  NSVfitTauToHadHypothesis* hypothesis = new NSVfitTauToHadHypothesis(particlePtr->second, hypothesisName, barcodeCounter_);

  const pat::Tau* tauPtr = dynamic_cast<const pat::Tau*>(particlePtr->second.get());
  assert(tauPtr);

  hypothesis->tracks_     = trackExtractor_(*tauPtr);

  hypothesis->p3Vis_unit_ = tauPtr->p4().Vect().Unit();
  const double visMassMax = SVfit_namespace::tauLeptonMass - 0.1;
  hypothesis->visMass_    = ( tauPtr->mass() < visMassMax ) ? tauPtr->mass() : visMassMax;

  NSVfitAlgorithmBase::fitParameterType* fitParameter = algorithm_->getFitParameter(prodParticleLabel_, kTau_nuInvMass);
  assert(fitParameter);
  fitParameter->upperLimit_ = SVfit_namespace::tauLeptonMass - hypothesis->visMass_;

  return hypothesis;
}

void NSVfitTauToHadBuilder::applyFitParameter(NSVfitSingleParticleHypothesisBase* hypothesis, double* param) const
{
  NSVfitTauToHadHypothesis* hypothesis_T = dynamic_cast<NSVfitTauToHadHypothesis*>(hypothesis);
  assert(hypothesis_T);

  double visEnFracX = param[idxFitParameter_visEnFracX_];
  double phi_lab    = param[idxFitParameter_phi_lab_];
  double pVis       = hypothesis_T->p4().P();
  double enVis_lab  = hypothesis_T->p4().energy();
  double visMass    = hypothesis_T->visMass();
  double nuInvMass  = 0.;

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
  double angleVis_lab = SVfit_namespace::gjAngleToLabFrame(pVis_rf, gjAngle, pVis);

//--- compute tau lepton momentum in laboratory frame
  double pTau_lab = SVfit_namespace::tauMomentumLabFrame(visMass, pVis_rf, gjAngle, pVis);

//--- compute tau lepton direction in laboratory frame
  reco::Candidate::Vector p3Tau = SVfit_namespace::tauDirection(p3Vis_unit, angleVis_lab, phi_lab);

//--- compute tau lepton four-vector in laboratory frame
  reco::Candidate::LorentzVector p4Tau = SVfit_namespace::tauP4(p3Tau.Unit(), pTau_lab);

  hypothesis_T->p4_fitted_      = p4Tau;
  hypothesis_T->dp4_            = (p4Tau - hypothesis_T->p4_);

  hypothesis_T->visEnFracX_     = visEnFracX;
  hypothesis_T->decay_angle_rf_ = gjAngle;
}

void NSVfitTauToHadBuilder::print(std::ostream& stream) const
{
  stream << "<NSVfitTauToHadBuilder::print>:" << std::endl;
  stream << " pluginName = " << pluginName_ << std::endl;
  stream << " pluginType = " << pluginType_ << std::endl;
  stream << " prodParticleLabel = " << prodParticleLabel_ << std::endl;
  stream << " idxFitParameter_visEnFracX = " << idxFitParameter_visEnFracX_ << std::endl;
  stream << " idxFitParameter_phi_lab = " << idxFitParameter_phi_lab_ << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleBuilderPluginFactory, NSVfitTauToHadBuilder, "NSVfitTauToHadBuilder");

