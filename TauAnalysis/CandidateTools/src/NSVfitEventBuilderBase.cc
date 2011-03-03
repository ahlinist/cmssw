#include "TauAnalysis/CandidateTools/interface/NSVfitEventBuilderBase.h"

NSVfitEventBuilderBase::NSVfitEventBuilderBase(const edm::ParameterSet& cfg) 
  : NSVfitBuilderBase(cfg.getParameter<edm::ParameterSet>("builder")),
    numResonanceBuilders_(0)
{
  edm::ParameterSet cfg_resonances = cfg.getParameter<edm::ParameterSet>("resonances");
  typedef std::vector<std::string> vstring;
  vstring resonanceNames = cfg_resonances.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator resonanceName = resonanceNames.begin();
	resonanceName != resonanceNames.end(); ++resonanceName ) {
    edm::ParameterSet cfg_resonance = cfg_resonances.getParameter<edm::ParameterSet>(*resonanceName);
    cfg_resonance.addParameter<std::string>("prodResonanceLabel", *resonanceName);
    std::string pluginType = cfg_resonance.getParameter<std::string>("pluginType");
    NSVfitResonanceBuilderBase* resonanceBuilder = 
      NSVfitResonanceBuilderPluginFactory::get()->create(pluginType, cfg_resonance);
    resonanceBuilders_.push_back(resonanceBuilder);
    ++numResonanceBuilders_;
  }
}

NSVfitEventBuilderBase::~NSVfitEventBuilderBase() 
{
  for ( std::vector<NSVfitResonanceBuilderBase*>::iterator it = resonanceBuilders_.begin();
	it != resonanceBuilders_.end(); ++it ) {
    delete (*it);
  }
}

void NSVfitEventBuilderBase::beginJob(NSVfitAlgorithmBase* algorithm) 
{
  for ( std::vector<NSVfitResonanceBuilderBase*>::iterator resonanceBuilder = resonanceBuilders_.begin();
	resonanceBuilder != resonanceBuilders_.end(); ++resonanceBuilder ) {
    (*resonanceBuilder)->beginJob(algorithm);
  }
}

void NSVfitEventBuilderBase::beginEvent(const edm::Event& evt, const edm::EventSetup& es) 
{
  NSVfitBuilderBase::beginEvent(evt, es);
  for ( std::vector<NSVfitResonanceBuilderBase*>::iterator resonanceBuilder = resonanceBuilders_.begin();
	resonanceBuilder != resonanceBuilders_.end(); ++resonanceBuilder ) {
    (*resonanceBuilder)->beginEvent(evt, es);
  }
}

NSVfitEventHypothesis* NSVfitEventBuilderBase::build(const inputParticleMap& inputParticles) const
{
  NSVfitEventHypothesis* eventHypothesis = new NSVfitEventHypothesis();

  for ( std::vector<NSVfitResonanceBuilderBase*>::const_iterator resonanceBuilder = resonanceBuilders_.begin();
	resonanceBuilder != resonanceBuilders_.end(); ++resonanceBuilder ) {
    NSVfitResonanceHypothesis* resonanceHypothesis = (*resonanceBuilder)->build(inputParticles);
    eventHypothesis->resonances_.push_back(resonanceHypothesis);
  }

  eventHypothesis->barcode_ = barcodeCounter_;
  ++barcodeCounter_;

  return eventHypothesis;
}

void NSVfitEventBuilderBase::applyFitParameter(NSVfitEventHypothesis* eventHypothesis, double* params) const
{
  for ( unsigned iResonanceBuilder = 0; iResonanceBuilder < numResonanceBuilders_; ++iResonanceBuilder ) {
    resonanceBuilders_[iResonanceBuilder]->applyFitParameter(eventHypothesis->resonances_[iResonanceBuilder], params);
  }
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
