#include "TauAnalysis/CandidateTools/interface/NSVfitEventBuilderBase.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesis.h"

NSVfitEventBuilderBase::NSVfitEventBuilderBase(const edm::ParameterSet& cfg)
  : NSVfitBuilderBase(cfg),
    numResonanceBuilders_(0),
    eventVertexRefitAlgorithm_(0)
{
  edm::ParameterSet cfg_resonances = cfg.getParameter<edm::ParameterSet>("resonances");
  typedef std::vector<std::string> vstring;
  vstring resonanceNames = cfg_resonances.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator resonanceName = resonanceNames.begin();
	resonanceName != resonanceNames.end(); ++resonanceName ) {
    edm::ParameterSet cfg_resonance = cfg_resonances.getParameter<edm::ParameterSet>(*resonanceName);
    edm::ParameterSet cfg_builder = cfg_resonance.getParameter<edm::ParameterSet>("builder");
    cfg_builder.addParameter<edm::ParameterSet>("daughters", cfg_resonance.getParameter<edm::ParameterSet>("daughters"));
    cfg_builder.addParameter<std::string>("prodResonanceLabel", *resonanceName);
    std::string pluginType = cfg_builder.getParameter<std::string>("pluginType");
    NSVfitResonanceBuilderBase* resonanceBuilder =
      NSVfitResonanceBuilderPluginFactory::get()->create(pluginType, cfg_builder);
    resonanceBuilders_.push_back(resonanceBuilder);
    ++numResonanceBuilders_;
  }

  eventVertexRefitAlgorithm_ = new NSVfitEventVertexRefitter(cfg);
}

NSVfitEventBuilderBase::~NSVfitEventBuilderBase()
{
  for ( std::vector<NSVfitResonanceBuilderBase*>::iterator it = resonanceBuilders_.begin();
	it != resonanceBuilders_.end(); ++it ) {
    delete (*it);
  }

  delete eventVertexRefitAlgorithm_;
}

void NSVfitEventBuilderBase::beginJob(NSVfitAlgorithmBase* algorithm)
{
  for ( std::vector<NSVfitResonanceBuilderBase*>::iterator resonanceBuilder = resonanceBuilders_.begin();
	resonanceBuilder != resonanceBuilders_.end(); ++resonanceBuilder ) {
    (*resonanceBuilder)->beginJob(algorithm);
  }

  idxFitParameter_pvShiftX_ = getFitParameterIdx(algorithm, "*", nSVfit_namespace::kPV_shiftX, true); // optional parameter
  idxFitParameter_pvShiftY_ = getFitParameterIdx(algorithm, "*", nSVfit_namespace::kPV_shiftY, true);
  idxFitParameter_pvShiftZ_ = getFitParameterIdx(algorithm, "*", nSVfit_namespace::kPV_shiftZ, true);
  if ( idxFitParameter_pvShiftX_ != -1 && idxFitParameter_pvShiftY_ != -1 && idxFitParameter_pvShiftZ_ != -1 )
    doEventVertexRefit_ = true;
  else
    doEventVertexRefit_ = false;
}

void NSVfitEventBuilderBase::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  NSVfitBuilderBase::beginEvent(evt, es);
  for ( std::vector<NSVfitResonanceBuilderBase*>::iterator resonanceBuilder = resonanceBuilders_.begin();
	resonanceBuilder != resonanceBuilders_.end(); ++resonanceBuilder ) {
    (*resonanceBuilder)->beginEvent(evt, es);
  }

  if ( doEventVertexRefit_ ) eventVertexRefitAlgorithm_->beginEvent(evt, es);
}

NSVfitEventHypothesis* NSVfitEventBuilderBase::build(const inputParticleMap& inputParticles, const reco::Vertex* eventVertex) const
{
  inputParticleMap::const_iterator metPtr = inputParticles.find("met");
  assert(metPtr != inputParticles.end());

  NSVfitEventHypothesis* event = new NSVfitEventHypothesis(metPtr->second);

  reco::Candidate::LorentzVector p4(0,0,0,0);

  for ( std::vector<NSVfitResonanceBuilderBase*>::const_iterator resonanceBuilder = resonanceBuilders_.begin();
	resonanceBuilder != resonanceBuilders_.end(); ++resonanceBuilder ) {
    NSVfitResonanceHypothesis* resonance = (*resonanceBuilder)->build(inputParticles);
    resonance->setEventHypothesis(event);

    p4 += resonance->p4();

    event->resonances_.push_back(resonance);
  }

  event->p4_ = p4;

  event->eventVertexIsValid_ = false;

//--- refit primary event vertex, excluding tracks of tau decay products
  if ( doEventVertexRefit_ ) {
    std::vector<const reco::Track*> svTracks;
    size_t numResonances = event->numResonances();
    for ( size_t iResonance = 0; iResonance < numResonances; ++iResonance ) {
      NSVfitResonanceHypothesis* resonance = event->resonance(iResonance);
      size_t numDaughters = resonance->numDaughters();
      for ( size_t iDaughter = 0; iDaughter < numDaughters; ++iDaughter ) {
	const NSVfitSingleParticleHypothesis* daughter = resonance->daughter(iDaughter);
	const NSVfitTauDecayHypothesis* tauDecayProducts = dynamic_cast<const NSVfitTauDecayHypothesis*>(daughter);
	if ( tauDecayProducts && tauDecayProducts->hasDecayVertex() )
	  svTracks.insert(svTracks.begin(), tauDecayProducts->tracks().begin(), tauDecayProducts->tracks().end());
      }
    }

    TransientVertex eventVertex_refitted = eventVertexRefitAlgorithm_->refit(eventVertex, &svTracks);
    if ( eventVertex_refitted.isValid() ) {
      event->eventVertexPosition_(0) = eventVertex_refitted.position().x();
      event->eventVertexPosition_(1) = eventVertex_refitted.position().y();
      event->eventVertexPosition_(2) = eventVertex_refitted.position().z();
      event->eventVertexPositionErr_ = eventVertex_refitted.positionError().matrix_new();
      // CV: need to add protection against case that primary event vertex is not valid <-- FIXME ?
      event->eventVertexIsValid_ = true;
    }
  } else {
    // Otherwise just take the position of the PV from the event.
    // Leave the errors @ 0.
    reco::Candidate::Point eventVertexPosition(0,0,0);
    if ( eventVertex ) eventVertexPosition = eventVertex->position();
    event->eventVertexPosition_(0) = eventVertexPosition.x();
    event->eventVertexPosition_(1) = eventVertexPosition.y();
    event->eventVertexPosition_(2) = eventVertexPosition.z();
    // CV: need to add protection against case that primary event vertex is not valid <-- FIXME ?
    event->eventVertexIsValid_ = true;
  }

  event->barcode_ = barcodeCounter_;
  ++barcodeCounter_;

  return event;
}

void NSVfitEventBuilderBase::applyFitParameter(NSVfitEventHypothesis* event, const double* param) const
{
  if ( doEventVertexRefit_ ) {
    double pvShiftX = param[idxFitParameter_pvShiftX_];
    double pvShiftY = param[idxFitParameter_pvShiftY_];
    double pvShiftZ = param[idxFitParameter_pvShiftZ_];
    event->eventVertexPositionShift_ = AlgebraicVector3(pvShiftX, pvShiftY, pvShiftZ);
  }

  reco::Candidate::LorentzVector dp4(0,0,0,0);

  for ( unsigned iResonanceBuilder = 0; iResonanceBuilder < numResonanceBuilders_; ++iResonanceBuilder ) {
    NSVfitResonanceHypothesis* resonance = event->resonance(iResonanceBuilder);
    resonanceBuilders_[iResonanceBuilder]->applyFitParameter(resonance, param);

    dp4 += resonance->dp4_fitted();
  }

  event->dp4_ = dp4;
}

void NSVfitEventBuilderBase::print(std::ostream& stream) const
{
  stream << "<NSVfitEventBuilderBase::print>:" << std::endl;
  stream << " pluginName = " << pluginName_ << std::endl;
  stream << " pluginType = " << pluginType_ << std::endl;
  for ( std::vector<NSVfitResonanceBuilderBase*>::const_iterator resonanceBuilder = resonanceBuilders_.begin();
	resonanceBuilder != resonanceBuilders_.end(); ++resonanceBuilder ) {
    (*resonanceBuilder)->print(stream);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(NSVfitEventBuilderPluginFactory, "NSVfitEventBuilderPluginFactory");
