#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToHadBuilder.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToHadHypothesis.h"

using namespace SVfit_namespace;

NSVfitTauToHadBuilder::NSVfitTauToHadBuilder(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleBuilderBase(cfg)
{}

NSVfitTauToHadBuilder::~NSVfitTauToHadBuilder()
{
// nothing to be done yet...
}

void NSVfitTauToHadBuilder::initialize(NSVfitAlgorithmBase* algorithm)
{
  idxFitParameter_theta_rf_ = getFitParameterIdx(algorithm, prodParticleLabel_, kTau_theta_rf);
  idxFitParameter_phi_lab_  = getFitParameterIdx(algorithm, prodParticleLabel_, kTau_phi_lab);
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
  hypothesis->visMass_    = tauPtr->mass();

  return hypothesis;
}

void NSVfitTauToHadBuilder::applyFitParameter(NSVfitSingleParticleHypothesisBase* hypothesis, double* param) const
{
  using namespace SVfit_namespace;

  NSVfitTauToHadHypothesis* hypothesis_T = dynamic_cast<NSVfitTauToHadHypothesis*>(hypothesis);
  assert(hypothesis_T);

  //double gjAngle    = param[idxFitParameter_theta_rf_];
  double visFractionX = -999; //FIXME GET FROM param[]
  double phi_lab    = param[idxFitParameter_phi_lab_];
  double pVis       = hypothesis_T->p4().P();
  double eVis_lab   = hypothesis_T->p4().energy();
  double visMass    = hypothesis_T->visMass();

  double nuInvMass  = 0.;

  const reco::Candidate::Vector& p3Vis_unit = hypothesis_T->p3Vis_unit();

//--- compute momentum of visible decay products in tau lepton rest frame
  double pVis_rf = pVisRestFrame(visMass, nuInvMass);
  double eVis_rf = energyFromMomentum(pVis_rf, tauLeptonMass);

  // Computing gjAngle as a function of X
  // First compute beta
  double beta = TMath::Sqrt(1 - square(tauLeptonMass*visFractionX/eVis_lab));
  double cosGjAngle = (tauLeptonMass*visFractionX-eVis_rf)/(pVis_rf*beta);
  double gjAngle = TMath::ACos(cosGjAngle);

//--- compute tau lepton decay angle in laboratory frame
  double angleVis_lab = SVfit_namespace::gjAngleToLabFrame(pVis_rf, gjAngle, pVis);

//--- compute tau lepton momentum in laboratory frame
  double pTau_lab = SVfit_namespace::tauMomentumLabFrame(visMass, pVis_rf, gjAngle, pVis);

//--- compute tau lepton direction in laboratory frame
  reco::Candidate::Vector p3Tau = SVfit_namespace::tauDirection(p3Vis_unit, angleVis_lab, phi_lab);

//--- compute tau lepton four-vector in laboratory frame
  reco::Candidate::LorentzVector p4Tau = SVfit_namespace::tauP4(p3Tau.Unit(), pTau_lab);

  hypothesis_T->p4_fitted_ = p4Tau;
  hypothesis_T->dp4_       = (p4Tau - hypothesis_T->p4_);

  hypothesis_T->decay_angle_rf_ = gjAngle;
}

void NSVfitTauToHadBuilder::print(std::ostream& stream) const
{
  stream << "<NSVfitTauToHadBuilder::print>:" << std::endl;
  stream << " pluginName = " << pluginName_ << std::endl;
  stream << " pluginType = " << pluginType_ << std::endl;
  stream << " prodParticleLabel = " << prodParticleLabel_ << std::endl;
  stream << " idxFitParameter_theta_rf = " << idxFitParameter_theta_rf_ << std::endl;
  stream << " idxFitParameter_phi_lab = " << idxFitParameter_phi_lab_ << std::endl;
}

