#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

using namespace SVfit_namespace;

const NSVfitAlgorithmBase* NSVfitAlgorithmBase::gNSVfitAlgorithm = 0;

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

  currentEventSetup_ = &es;
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

  NSVfitParameter* fitParameter = getFitParameter(name, type);

  if ( !fitParameter ) {
    assert(type >= 0 && type <= nSVfit_namespace::kNu_phi_lab);
    NSVfitParameter newFitParameter(fitParameterCounter_, name, type);
    fitParameters_.push_back(newFitParameter);
    fitParameter = &fitParameters_.back();
    ++fitParameterCounter_;
  }

  fitParameter->regUsedBy(requester);
}

NSVfitParameter* NSVfitAlgorithmBase::getFitParameter(const std::string& name, int type) const
{
  NSVfitParameter* retVal = 0;

  for ( std::vector<NSVfitParameter>::iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    if ( fitParameter->Name() == name && fitParameter->Type() == type ) retVal = &(*fitParameter);
  }
  
  return retVal;
}

NSVfitParameter* NSVfitAlgorithmBase::getFitParameter(int idx) const
{
  assert(idx >= 0 && idx < (int)fitParameters_.size());

  NSVfitParameter* retVal = &fitParameters_[idx];
  assert(retVal && retVal->index() == idx);

  return retVal;
}

NSVfitEventHypothesisBase* NSVfitAlgorithmBase::fit(const inputParticleMap& inputParticles, const reco::Vertex* eventVertex) const
{
  // beginEvent should always be called before fit(...)
  assert(currentEventSetup_);

  // Setup the track service
  trackService_->setup(*currentEventSetup_, eventVertex->position());

  currentEventHypothesis_ = eventModel_->builder_->build(inputParticles, eventVertex);
  currentEventHypothesis_->name_ = pluginName_;

  eventModel_->beginCandidate(currentEventHypothesis_);

  gNSVfitAlgorithm = this;

  fitImp();

  return fittedEventHypothesis_;
}

double NSVfitAlgorithmBase::nll(const double* x, const double* param) const
{
  eventModel_->builder_->applyFitParameter(currentEventHypothesis_, x);
  return eventModel_->nll(currentEventHypothesis_);
}

void NSVfitAlgorithmBase::setMassResults(NSVfitResonanceHypothesisBase* resonance, double value, double errUp, double errDown) const
{
  resonance->mass_ = value;
  resonance->massErrUp_ = errUp;
  resonance->massErrDown_ = errDown;
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(NSVfitAlgorithmPluginFactory, "NSVfitAlgorithmPluginFactory");


