#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TMath.h>

using namespace SVfit_namespace;

const NSVfitAlgorithmBase* NSVfitAlgorithmBase::gNSVfitAlgorithm = 0;

typedef std::pair<double, double> pdouble;
std::vector<pdouble> NSVfitAlgorithmBase::fitParameterLimits_;

void initializeFitParameterLimits(std::vector<pdouble>& limits)
{
  limits[kPV_shiftX]             = pdouble(         -1.,          +1.); // cm
  limits[kPV_shiftY]             = pdouble(         -1.,          +1.); // cm
  limits[kPV_shiftZ]             = pdouble(        -10.,         +10.); // cm
  limits[kTau_visEnFracX]        = pdouble(          0.,           1.); // dimensionless
  limits[kTau_phi_lab]           = pdouble(-TMath::Pi(), +TMath::Pi()); // rad
  limits[kTau_decayDistance_lab] = pdouble(          0.,          10.); // cm
  limits[kTau_nuInvMass]         = pdouble(          0.,           0.); // depends on decay: mMau - mVis
  limits[kTauVM_theta_rho]       = pdouble(          0.,  TMath::Pi()); // rad
  limits[kTauVM_theta_a1]        = pdouble(          0.,  TMath::Pi()); // rad
  limits[kTauVM_theta_a1r]       = pdouble(          0.,  TMath::Pi()); // rad
  limits[kTauVM_phi_a1r]         = pdouble(-TMath::Pi(), +TMath::Pi()); // rad
  limits[kLep_shiftEn]           = pdouble(          0.,          10.); // relative to measured lepton energy
  limits[kNu_energy_lab]         = pdouble(          0.,        1.e+3); // GeV
  limits[kNu_phi_lab]            = pdouble(          0.,  TMath::Pi()); // rad
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
  eventModel_->builder_->beginJob(this);
  
  for ( std::vector<NSVfitLikelihoodBase*>::iterator likelihood = allLikelihoods_.begin();
	likelihood != allLikelihoods_.end(); ++likelihood ) {
    (*likelihood)->beginJob(this);
  }
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
    if ( fitParameter->name_ == name && fitParameter->type_ == type ) retVal = &(* fitParameter);
  }

  return retVal;
}

NSVfitEventHypothesis* NSVfitAlgorithmBase::fit(const inputParticleMap& inputParticles) const
{
  currentEventHypothesis_ = eventModel_->builder_->build(inputParticles);

  gNSVfitAlgorithm = this;

  fitImp();

  return currentEventHypothesis_;
}

double NSVfitAlgorithmBase::nll(double* x, double* param) const
{
  eventModel_->builder_->applyFitParameter(currentEventHypothesis_, x);
  return eventModel_->nll(currentEventHypothesis_);
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(NSVfitAlgorithmPluginFactory, "NSVfitAlgorithmPluginFactory");


