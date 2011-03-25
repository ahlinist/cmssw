#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

using namespace SVfit_namespace;

const NSVfitAlgorithmBase* NSVfitAlgorithmBase::gNSVfitAlgorithm = 0;

typedef std::pair<double, double> pdouble;
std::vector<pdouble> NSVfitAlgorithmBase::fitParameterLimits_;

void initializeFitParameterLimits(std::vector<pdouble>& limits)
{
  limits.resize(nSVfit_namespace::kNu_phi_lab + 1);
  limits[nSVfit_namespace::kPV_shiftX]             = pdouble(         -1.,          +1.); // cm
  limits[nSVfit_namespace::kPV_shiftY]             = pdouble(         -1.,          +1.); // cm
  limits[nSVfit_namespace::kPV_shiftZ]             = pdouble(        -10.,         +10.); // cm
  limits[nSVfit_namespace::kTau_visEnFracX]        = pdouble(          0.,           1.); // dimensionless
  limits[nSVfit_namespace::kTau_phi_lab]           = pdouble(-TMath::Pi(), +TMath::Pi()); // rad
  limits[nSVfit_namespace::kTau_decayDistance_lab] = pdouble(          0.,          10.); // cm
  limits[nSVfit_namespace::kTau_nuInvMass]         = pdouble(          0.,           0.); // depends on decay: mMau - mVis
  limits[nSVfit_namespace::kTau_pol]               = pdouble(         -1.,          +1.); // -1: left-handed, +1: right-handed
  limits[nSVfit_namespace::kTauVM_theta_rho]       = pdouble(          0.,  TMath::Pi()); // rad
  limits[nSVfit_namespace::kTauVM_mass2_rho]       = pdouble(square(chargedPionMass +   neutralPionMass), tauLeptonMass2); // GeV^2
  limits[nSVfit_namespace::kTauVM_theta_a1]        = pdouble(          0.,  TMath::Pi()); // rad
  limits[nSVfit_namespace::kTauVM_theta_a1r]       = pdouble(          0.,  TMath::Pi()); // rad
  limits[nSVfit_namespace::kTauVM_phi_a1r]         = pdouble(-TMath::Pi(), +TMath::Pi()); // rad
  limits[nSVfit_namespace::kTauVM_mass2_a1]        = pdouble(square(chargedPionMass + 2*neutralPionMass), tauLeptonMass2); // GeV^2
  limits[nSVfit_namespace::kLep_shiftEn]           = pdouble(          0.,          10.); // relative to measured lepton energy
  limits[nSVfit_namespace::kNu_energy_lab]         = pdouble(          0.,        1.e+3); // GeV
  limits[nSVfit_namespace::kNu_phi_lab]            = pdouble(          0.,  TMath::Pi()); // rad
}

NSVfitAlgorithmBase::NSVfitAlgorithmBase(const edm::ParameterSet& cfg)
  : currentEventHypothesis_(0),
    fitParameterCounter_(0)
{
  pluginName_ = cfg.getParameter<std::string>("pluginName");
  pluginType_ = cfg.getParameter<std::string>("pluginType");

  edm::ParameterSet cfgEvent = cfg.getParameter<edm::ParameterSet>("event");
  eventModel_ = new eventModelType(cfgEvent, allLikelihoods_);

  verbosity_ = cfg.exists("verbosity") ? 
    cfg.getParameter<int>("verbosity") : 0;

  initializeFitParameterLimits(fitParameterLimits_);
}

NSVfitAlgorithmBase::~NSVfitAlgorithmBase() 
{
  delete eventModel_;
}

void NSVfitAlgorithmBase::beginJob() 
{
  for ( std::vector<NSVfitLikelihoodBase*>::iterator likelihood = allLikelihoods_.begin();
	likelihood != allLikelihoods_.end(); ++likelihood ) {
    (*likelihood)->beginJob(this);
  }

  eventModel_->builder_->beginJob(this);
}

void NSVfitAlgorithmBase::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  for ( std::vector<NSVfitLikelihoodBase*>::iterator likelihood = allLikelihoods_.begin();
	likelihood != allLikelihoods_.end(); ++likelihood ) {
    (*likelihood)->beginEvent(evt, es);
  }
}  

void NSVfitAlgorithmBase::requestFitParameter(const std::string& name, int type, const std::string& requester)
{
  if ( name == "allTauDecays" ||
       name == "allLeptons"   ||
       name == "allNeutrinos" ) {
    edm::LogWarning ("NSVfitAlgorithmBase::requestFitParameter")
      << " Value = " << name << " not supported yet" 
      << " --> relying on SingleParticleLikelihood plugins to initialize fitParameter for now.";
    return;
  }

  fitParameterType* fitParameter = getFitParameter(name, type);

  if ( !fitParameter ) {
    fitParameterType newFitParameter;
    newFitParameter.name_ = name;
    newFitParameter.type_ = type;
    assert(type >= 0 && type < (int)fitParameterLimits_.size());
    newFitParameter.lowerLimit_ = fitParameterLimits_[type].first;
    newFitParameter.upperLimit_ = fitParameterLimits_[type].second; 
    newFitParameter.idx_ = fitParameterCounter_;
    fitParameters_.push_back(newFitParameter);
    fitParameter = &fitParameters_.back();
    ++fitParameterCounter_;
  }

  fitParameter->usedBy_.push_back(requester);
}

NSVfitAlgorithmBase::fitParameterType* NSVfitAlgorithmBase::getFitParameter(const std::string& name, int type) 
{
  fitParameterType* retVal = 0;

  for ( std::vector<fitParameterType>::iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    if ( fitParameter->name_ == name && fitParameter->type_ == type ) retVal = &(*fitParameter);
  }

  return retVal;
}

NSVfitEventHypothesis* NSVfitAlgorithmBase::fit(const inputParticleMap& inputParticles, const reco::Vertex* eventVertex) const
{
  currentEventHypothesis_ = eventModel_->builder_->build(inputParticles, eventVertex);

  eventModel_->beginCandidate(currentEventHypothesis_);

  gNSVfitAlgorithm = this;

  fitImp();

  return currentEventHypothesis_;
}

double NSVfitAlgorithmBase::nll(double* x, double* param) const
{
  eventModel_->builder_->applyFitParameter(currentEventHypothesis_, x);
  return eventModel_->nll(currentEventHypothesis_);
}

void NSVfitAlgorithmBase::setMassResults(NSVfitResonanceHypothesis* resonance, double value, double errUp, double errDown) const
{
  resonance->mass_ = value;
  resonance->massErrUp_ = errUp;
  resonance->massErrDown_ = errDown;
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(NSVfitAlgorithmPluginFactory, "NSVfitAlgorithmPluginFactory");


