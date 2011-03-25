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
  idxFitParameter_visEnFracX_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX);
  idxFitParameter_phi_lab_    = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_phi_lab);

  idxFitParameter_pol_        = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_pol, true); // optional parameter

  idxFitParameter_thetaVMrho_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_theta_rho, true);
  idxFitParameter_mass2VMrho_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_mass2_rho, true);
  idxFitParameter_thetaVMa1_  = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_theta_a1,  true);
  idxFitParameter_thetaVMa1r_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_theta_a1r, true);
  idxFitParameter_phiVMa1r_   = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_phi_a1r,   true); 
  idxFitParameter_mass2VMa1_  = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTauVM_mass2_a1,  true);

  algorithm_ = algorithm;
}

NSVfitSingleParticleHypothesisBase* NSVfitTauToHadBuilder::build(const inputParticleMap& inputParticles) const
{
  inputParticleMap::const_iterator particlePtr = inputParticles.find(prodParticleLabel_);
  assert(particlePtr != inputParticles.end());

  NSVfitTauToHadHypothesis* hypothesis = new NSVfitTauToHadHypothesis(particlePtr->second, prodParticleLabel_, barcodeCounter_);
  ++barcodeCounter_;

  const pat::Tau* tauPtr = dynamic_cast<const pat::Tau*>(particlePtr->second.get());
  assert(tauPtr);

  hypothesis->tracks_     = trackExtractor_(*tauPtr);
  hypothesis->p3Vis_unit_ = tauPtr->p4().Vect().Unit();
  const double visMassMax = SVfit_namespace::tauLeptonMass - 0.1;
  hypothesis->visMass_    = ( tauPtr->mass() < visMassMax ) ? tauPtr->mass() : visMassMax;

  hypothesis->decayMode_  = tauPtr->decayMode();

  return hypothesis;
}

void applyOptionalFitParameter(double* param, int idxFitParameter, double& value)
{
  if   ( idxFitParameter != -1 ) value = param[idxFitParameter];
  else                           value = 0.;
}

void NSVfitTauToHadBuilder::applyFitParameter(NSVfitSingleParticleHypothesisBase* hypothesis, double* param) const
{
  NSVfitTauToHadHypothesis* hypothesis_T = dynamic_cast<NSVfitTauToHadHypothesis*>(hypothesis);
  assert(hypothesis_T);

  double visEnFracX = param[idxFitParameter_visEnFracX_];
  double phi_lab    = param[idxFitParameter_phi_lab_];
  double pVis_lab   = hypothesis_T->p4().P();
  double enVis_lab  = hypothesis_T->p4().energy();
  double visMass    = hypothesis_T->visMass();
  double nuInvMass  = 0.;

  const reco::Candidate::Vector& p3Vis_unit = hypothesis_T->p3Vis_unit();

//--- compute momentum of visible decay products in tau lepton rest frame
  double pVis_rf = SVfit_namespace::pVisRestFrame(visMass, nuInvMass);

//--- decay angle in tau lepton rest frame as function of X
//    (= energy ratio of visible decay products/tau lepton energy)
  double gjAngle = SVfit_namespace::gjAngleFromX(visEnFracX, visMass, pVis_rf, enVis_lab);

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

  applyOptionalFitParameter(param, idxFitParameter_pol_,        hypothesis_T->polarization_);

  applyOptionalFitParameter(param, idxFitParameter_thetaVMrho_, hypothesis_T->decay_angle_VMrho_);
  applyOptionalFitParameter(param, idxFitParameter_mass2VMrho_, hypothesis_T->mass2_VMrho_);
  applyOptionalFitParameter(param, idxFitParameter_thetaVMa1_,  hypothesis_T->decay_angle_VMa1_);
  applyOptionalFitParameter(param, idxFitParameter_thetaVMa1r_, hypothesis_T->decay_angle_VMa1r_theta_);
  applyOptionalFitParameter(param, idxFitParameter_phiVMa1r_,   hypothesis_T->decay_angle_VMa1r_phi_);
  applyOptionalFitParameter(param, idxFitParameter_mass2VMa1_,  hypothesis_T->mass2_VMa1_);

  if ( verbosity_ ) {
    std::cout << "<NSVfitTauToHadBuilder::applyFitParameter>:" << std::endl;
    std::cout << " visEnFracX = " << param[idxFitParameter_visEnFracX_] << std::endl;
    std::cout << " phi_lab = " << param[idxFitParameter_phi_lab_] << std::endl;
    std::cout << " enVis_lab = " << enVis_lab << std::endl;
    std::cout << " visMass = " << visMass << std::endl;
    std::cout << " gjAngle = " << gjAngle << std::endl;
    std::cout << " angleVis_lab = " << angleVis_lab << std::endl;
    std::cout << " pTau_lab = " << pTau_lab << std::endl;
    std::cout << "p4Vis: E = " << hypothesis_T->p4_.energy() << ","
	      << " px = " << hypothesis_T->p4_.px() << ", py = " << hypothesis_T->p4_.py() << ","
	      << " pz = " << hypothesis_T->p4_.pz() << std::endl;
    std::cout << "p4Tau: E = " << p4Tau.energy() << ","
	      << " px = " << p4Tau.px() << ", py = " << p4Tau.py() << ","
	      << " pz = " << p4Tau.pz() << std::endl;
    std::cout << "polarization = " << hypothesis_T->polarization_ << std::endl;
  }

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
  stream << " idxFitParameter_pol = " << idxFitParameter_pol_ << std::endl;
  stream << " idxFitParameter_thetaVMrho = " << idxFitParameter_thetaVMrho_ << std::endl;
  stream << " idxFitParameter_mass2VMrho = " << idxFitParameter_mass2VMrho_ << std::endl;
  stream << " idxFitParameter_thetaVMa1 = " << idxFitParameter_thetaVMa1_ << std::endl;
  stream << " idxFitParameter_thetaVMa1r = " << idxFitParameter_thetaVMa1r_ << std::endl;
  stream << " idxFitParameter_phiVMa1r = " << idxFitParameter_phiVMa1r_ << std::endl;
  stream << " idxFitParameter_mass2VMa1 = " << idxFitParameter_mass2VMa1_ << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleBuilderPluginFactory, NSVfitTauToHadBuilder, "NSVfitTauToHadBuilder");

