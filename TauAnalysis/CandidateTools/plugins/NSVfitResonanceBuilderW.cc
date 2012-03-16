#include "TauAnalysis/CandidateTools/plugins/NSVfitResonanceBuilderW.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitStableSingleParticleHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleNeutrinoHypothesis.h"

#include <vector>
#include <string>

using namespace SVfit_namespace;

NSVfitResonanceBuilderW::NSVfitResonanceBuilderW(const edm::ParameterSet& cfg)
  : NSVfitResonanceBuilderBase(cfg),
    W_mass_(cfg.getParameter<double>("W_mass")),
    W_width_(cfg.getParameter<double>("W_width"))
{
  edm::ParameterSet cfg_daughters = cfg.getParameter<edm::ParameterSet>("daughters");
  std::vector<std::string> daughterNames = cfg_daughters.getParameterNamesForType<edm::ParameterSet>();
  if ( daughterNames.size() == 1 ) {
    prodChargedLeptonLabel_ = daughterNames.front();
  } else {
    throw cms::Exception("NSVfitResonanceBuilderW")
      << " Exactly one daughter required !!\n" << std::endl;	
  }

  prodNeutrinoLabel_ = std::string(prodResonanceLabel_).append("_neutrino");
}

void NSVfitResonanceBuilderW::beginJob(NSVfitAlgorithmBase* algorithm)
{
  // CV: do not rely on any likelihood function to request the W boson specific fit parameter
  algorithm->requestFitParameter(prodResonanceLabel_, nSVfit_namespace::kW_theta_lab, pluginName_);
  algorithm->requestFitParameter(prodResonanceLabel_, nSVfit_namespace::kW_phi_lab,   pluginName_);
  algorithm->requestFitParameter(prodResonanceLabel_, nSVfit_namespace::kW_mass,      pluginName_);

  idxFitParameter_theta_lab_ = getFitParameterIdx(algorithm, prodResonanceLabel_, nSVfit_namespace::kW_theta_lab);
  idxFitParameter_phi_lab_   = getFitParameterIdx(algorithm, prodResonanceLabel_, nSVfit_namespace::kW_phi_lab);
  idxFitParameter_mass_      = getFitParameterIdx(algorithm, prodResonanceLabel_, nSVfit_namespace::kW_mass);

  NSVfitParameter* fitParameter = algorithm->getFitParameter(idxFitParameter_mass_);
  assert(fitParameter);
  fitParameter->setInitialValue(W_mass_);
  fitParameter->setLowerLimit(W_mass_ - 3.*W_width_);
  fitParameter->setUpperLimit(W_mass_ + 3.*W_width_);
}

NSVfitResonanceHypothesis* NSVfitResonanceBuilderW::build(const inputParticleMap& inputParticles) const
{
  if ( this->verbosity_ ) std::cout << "<NSVfitResonanceBuilderW::build>:" << std::endl;

  inputParticleMap::const_iterator particlePtr = inputParticles.find(prodChargedLeptonLabel_);
  assert(particlePtr != inputParticles.end());

  NSVfitStableSingleParticleHypothesis* chargedLepton = 
    new NSVfitStableSingleParticleHypothesis(particlePtr->second, prodChargedLeptonLabel_, barcodeCounter_);
  chargedLepton->p3_unit_ = chargedLepton->p4().Vect().Unit();

  NSVfitSingleNeutrinoHypothesis* neutrino = 
    new NSVfitSingleNeutrinoHypothesis(prodNeutrinoLabel_, barcodeCounter_);

  NSVfitResonanceHypothesis* resonance = new NSVfitResonanceHypothesis();
  resonance->daughters_.push_back(chargedLepton);
  resonance->daughters_.push_back(neutrino);
  resonance->p4_ = chargedLepton->p4();
  resonance->name_ = prodResonanceLabel_;

  ++barcodeCounter_; 

  return resonance;
}

void NSVfitResonanceBuilderW::applyFitParameter(NSVfitResonanceHypothesis* resonance, const double* param) const
{
  assert(resonance->numDaughters() == 2);

  NSVfitStableSingleParticleHypothesis* chargedLepton = dynamic_cast<NSVfitStableSingleParticleHypothesis*>(resonance->daughter(0));

  NSVfitSingleNeutrinoHypothesis* neutrino = dynamic_cast<NSVfitSingleNeutrinoHypothesis*>(resonance->daughter(1));

  double gjAngle = param[idxFitParameter_theta_lab_];
  double phi_lab = param[idxFitParameter_phi_lab_];
  double W_mass  = param[idxFitParameter_mass_];

  const reco::Candidate::LorentzVector& p4ChargedLepton_lab = chargedLepton->p4();
  double pChargedLepton_lab  = p4ChargedLepton_lab.P();
  double massChargedLepton   = p4ChargedLepton_lab.mass();

  const reco::Candidate::Vector& p3ChargedLepton_unit = chargedLepton->p3_unit();

//--- compute momentum of charged lepton in W boson rest frame
  double pChargedLepton_rf = pVisRestFrame(massChargedLepton, 0., W_mass);

//--- compute W boson decay angle in laboratory frame
  double angleVis_lab = gjAngleToLabFrame(pChargedLepton_rf, gjAngle, pChargedLepton_lab);

//--- compute W boson momentum in laboratory frame
  double pW_lab = motherMomentumLabFrame(massChargedLepton, pChargedLepton_rf, gjAngle, pChargedLepton_lab, W_mass);

//--- compute W boson four-vector in laboratory frame
  reco::Candidate::Vector Wdirection = motherDirection(p3ChargedLepton_unit, angleVis_lab, phi_lab);
  reco::Candidate::LorentzVector p4W_lab = motherP4(Wdirection.Unit(), pW_lab, W_mass);

  const reco::Candidate::LorentzVector& p4Neutrino_lab = p4W_lab - p4ChargedLepton_lab;;
  neutrino->p4_fitted_ = p4Neutrino_lab;
  neutrino->dp4_       = neutrino->p4_fitted_;

  if ( verbosity_ ) {
    std::cout << "<NSVfitTauDecayBuilderBase::applyFitParameter>:" << std::endl;
    std::cout << " qjAngle = " << gjAngle << std::endl;
    std::cout << " phi_lab = " << phi_lab << std::endl;
    std::cout << " W_mass = " << W_mass << std::endl;
    std::cout << "p4ChargedLepton: E = " << p4ChargedLepton_lab.E() << ","
	      << " px = " << p4ChargedLepton_lab.px() << ", py = " << p4ChargedLepton_lab.py() << ","
	      << " pz = " << p4ChargedLepton_lab.pz() << std::endl;
    std::cout << "p4Neutrino: E = " << p4Neutrino_lab.E() << ","
	      << " px = " << p4Neutrino_lab.px() << ", py = " << p4Neutrino_lab.py() << ","
	      << " pz = " << p4Neutrino_lab.pz() << std::endl;
    std::cout << "p4W: E = " << p4W_lab.E() << ","
	      << " px = " << p4W_lab.px() << ", py = " << p4W_lab.py() << ","
	      << " pz = " << p4W_lab.pz() << std::endl;
  }
  
  resonance->dp4_ = chargedLepton->dp4_fitted() + neutrino->dp4_fitted();
  resonance->prod_angle_rf_ = gjAngle;
}

void NSVfitResonanceBuilderW::print(std::ostream& stream) const
{
  stream << "<NSVfitResonanceBuilderBase::print>:" << std::endl;
  stream << " pluginName = " << pluginName_ << std::endl;
  stream << " pluginType = " << pluginType_ << std::endl;
  stream << " prodResonanceLabel = " << prodResonanceLabel_ << std::endl;
  stream << " prodChargedLeptonLabel = " << prodChargedLeptonLabel_ << std::endl;
  stream << " prodNeutrinoLabel = " << prodNeutrinoLabel_ << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitResonanceBuilderPluginFactory, NSVfitResonanceBuilderW, "NSVfitResonanceBuilderW");
