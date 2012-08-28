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

  eventModel_->builder_->beginEvent(evt, es);

  currentEventSetup_ = &es;
}

void NSVfitAlgorithmBase::requestFitParameter(const std::string& name, int type, const std::string& requester)
{
  if ( name == "allTauDecays" ||
       name == "allLeptons"   ||
       name == "allNeutrinos" ) {
    //edm::LogWarning ("NSVfitAlgorithmBase::requestFitParameter")
    //  << " Value = " << name << " not supported yet"
    //  << " --> relying on SingleParticleLikelihood plugins to initialize fitParameter for now.";
    return;
  }

  NSVfitParameter* fitParameter = getFitParameter(name, type);

  if ( !fitParameter ) {
    assert(type >= 0 && type < nSVfit_namespace::kNumFitParameter);
    NSVfitParameter newFitParameter(fitParameterCounter_, name, type);
    fitParameters_.push_back(newFitParameter);
    fitParameter = &fitParameters_.back();
    ++fitParameterCounter_;
  }

  fitParameter->regUsedBy(requester);
}

void NSVfitAlgorithmBase::fixFitParameter(int fitParameterIdx)
{
  if ( fitParameterIdx != -1 ) {
    NSVfitParameter* fitParameter = this->getFitParameter(fitParameterIdx);
    assert(fitParameter);
    fitParameter->setIsFixed(true);
  }
}

unsigned NSVfitAlgorithmBase::getNumFitParameter(const std::string& name) const
{
  unsigned retVal = 0;

  for ( std::vector<NSVfitParameter>::iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    if ( fitParameter->Name() == name ) ++retVal;
  }

  return retVal;
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

void NSVfitAlgorithmBase::setFitParameterInitialValue(int fitParameterIdx, double initialValue)
{
  if ( fitParameterIdx != -1 ) {
    NSVfitParameter* fitParameter = this->getFitParameter(fitParameterIdx);
    assert(fitParameter);
    fitParameter->setInitialValue(initialValue);
  }
}

void NSVfitAlgorithmBase::setFitParameterLimit(int fitParameterIdx, double lowerLimit, double upperLimit)
{
  if ( fitParameterIdx != -1 ) {
    NSVfitParameter* fitParameter = this->getFitParameter(fitParameterIdx);
    assert(fitParameter);
    fitParameter->setLowerLimit(lowerLimit);
    fitParameter->setUpperLimit(upperLimit);
  }
}

void NSVfitAlgorithmBase::setFitParameterStepSize(int fitParameterIdx, double stepSize)
{
  if ( fitParameterIdx != -1 ) {
    NSVfitParameter* fitParameter = this->getFitParameter(fitParameterIdx);
    assert(fitParameter);
    fitParameter->setStepSize(stepSize);
  }
}

NSVfitEventHypothesisBase* NSVfitAlgorithmBase::fit(const inputParticleMap& inputParticles, const reco::Vertex* eventVertex) const
{
  // beginEvent should always be called before fit(...)
  assert(currentEventSetup_);

  // Setup the track service
  reco::Candidate::Point eventVertexPosition(0,0,0);
  if ( eventVertex ) eventVertexPosition = eventVertex->position();
  trackService_->setup(*currentEventSetup_, eventVertexPosition);

  currentEventHypothesis_ = eventModel_->builder_->build(inputParticles, eventVertex);
  currentEventHypothesis_->name_ = pluginName_;
  currentEventHypothesis_isValidSolution_ = true;

  eventModel_->beginCandidate(currentEventHypothesis_);

  gNSVfitAlgorithm = this;

  if ( verbosity_ >= 1 ) {
    std::cout << "<NSVfitAlgorithmBase::fit>:" << std::endl;
    for ( std::vector<NSVfitParameter>::const_iterator fitParameter = fitParameters_.begin();
	  fitParameter != fitParameters_.end(); ++fitParameter ) {
      fitParameter->dump(std::cout);
    }
  }

  fitImp();
  fittedEventHypothesis_->nll_ = fittedEventHypothesis_nll_;
  if ( verbosity_ >= 2 ) fittedEventHypothesis_->print(std::cout);

  return fittedEventHypothesis_;
}

bool NSVfitAlgorithmBase::update(const double* x, const double* param) const
{
  currentEventHypothesis_isValidSolution_ = eventModel_->builder_->applyFitParameter(currentEventHypothesis_, x);
  if ( verbosity_ >= 2 ) {
    currentEventHypothesis_->print(std::cout);
    std::cout << "isValidSolution = " << currentEventHypothesis_isValidSolution_ << std::endl;
  }
  return currentEventHypothesis_isValidSolution_;
}

double NSVfitAlgorithmBase::nll(const double* x, const double* param) const
{
  update(x, param);

  double nll = eventModel_->nll(currentEventHypothesis_);
  if ( TMath::IsNaN(nll) ) nll = std::numeric_limits<float>::max();

  return nll;
}

void NSVfitAlgorithmBase::setMassResults(NSVfitResonanceHypothesisBase* resonance, double value, double errUp, double errDown) const
{
  resonance->mass_ = value;
  resonance->massErrUp_ = errUp;
  resonance->massErrDown_ = errDown;
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(NSVfitAlgorithmPluginFactory, "NSVfitAlgorithmPluginFactory");


