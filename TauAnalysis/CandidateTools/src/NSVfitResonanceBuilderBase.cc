#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceBuilderBase.h"

NSVfitResonanceBuilderBase::NSVfitResonanceBuilderBase(const edm::ParameterSet& cfg) 
  : NSVfitBuilderBase(cfg.getParameter<edm::ParameterSet>("builder")),
    numDaughterBuilders_(0)
{
  edm::ParameterSet cfg_daughters = cfg.getParameter<edm::ParameterSet>("daughters");
  typedef std::vector<std::string> vstring;
  vstring daughterNames = cfg_daughters.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator daughterName = daughterNames.begin();
	daughterName != daughterNames.end(); ++daughterName ) {
    edm::ParameterSet cfg_daughter = cfg_daughters.getParameter<edm::ParameterSet>(*daughterName);
    cfg_daughter.addParameter<std::string>("prodParticleLabel", *daughterName);
    std::string pluginType = cfg_daughter.getParameter<std::string>("pluginType");
    NSVfitSingleParticleBuilderBase* daughterBuilder = 
      NSVfitSingleParticleBuilderPluginFactory::get()->create(pluginType, cfg_daughter);
    daughterBuilders_.push_back(daughterBuilder);
    ++numDaughterBuilders_;
  }
}

NSVfitResonanceBuilderBase::~NSVfitResonanceBuilderBase() 
{
  for ( std::vector<NSVfitSingleParticleBuilderBase*>::iterator it = daughterBuilders_.begin();
	it != daughterBuilders_.end(); ++it ) {
    delete (*it);
  }
}

void NSVfitResonanceBuilderBase::beginJob(NSVfitAlgorithmBase* algorithm) 
{
  for ( std::vector<NSVfitSingleParticleBuilderBase*>::iterator daughterBuilder = daughterBuilders_.begin();
	daughterBuilder != daughterBuilders_.end(); ++daughterBuilder ) {
    (*daughterBuilder)->beginJob(algorithm);
  }
}

void NSVfitResonanceBuilderBase::beginEvent(const edm::Event& evt, const edm::EventSetup& es) 
{
  NSVfitBuilderBase::beginEvent(evt, es);
  for ( std::vector<NSVfitSingleParticleBuilderBase*>::iterator daughterBuilder = daughterBuilders_.begin();
	daughterBuilder != daughterBuilders_.end(); ++daughterBuilder ) {
    (*daughterBuilder)->beginEvent(evt, es);
  }
}

NSVfitResonanceHypothesis* NSVfitResonanceBuilderBase::build(const inputParticleMap& inputParticles) const
{
/*
  NSVfitResonanceHypothesis* resonanceHypothesis = new NSVfitResonanceHypothesis();

  for ( std::map<std::string, NSVfitSingleParticleBuilderBase*>::const_iterator daughterBuilder = daughterBuilders_.begin();
	daughterBuilder != daughterBuilders_.end(); ++daughterBuilder ) {
    NSVfitSingleParticleHypothesisBase* daughterHypothesis = 0;

    inputParticleMap::const_iterator daughterParticle = inputParticles.find((*daughterBuilder)->inputParticleLabel());
    if ( daughterParticle != inputParticles.end() ) {
      daughterHypothesis = (*daughterBuilder)->build(daughterParticle->second());
    } else {
      daughterHypothesis = (*daughterBuilder)->build();
    }

    resonanceHypothesis->daughters_.push_back(daughterHypothesis);
  }

  resonanceHypothesis->barcode_ = barcodeCounter_;

  ++barcodeCounter_;
 */
  NSVfitResonanceHypothesis* resonanceHypothesis = new NSVfitResonanceHypothesis();

  for ( std::vector<NSVfitSingleParticleBuilderBase*>::const_iterator daughterBuilder = daughterBuilders_.begin();
	daughterBuilder != daughterBuilders_.end(); ++daughterBuilder ) {
    NSVfitSingleParticleHypothesisBase* daughterHypothesis = (*daughterBuilder)->build(inputParticles);
    resonanceHypothesis->daughters_.push_back(daughterHypothesis);
  }

  resonanceHypothesis->barcode_ = barcodeCounter_;
  ++barcodeCounter_;

  return resonanceHypothesis;
}

void NSVfitResonanceBuilderBase::applyFitParameter(NSVfitResonanceHypothesis* resonanceHypothesis, double* params) const
{
  for ( unsigned iDaughterBuilder = 0; iDaughterBuilder < numDaughterBuilders_; ++iDaughterBuilder ) {
    daughterBuilders_[iDaughterBuilder]->applyFitParameter(resonanceHypothesis->daughters_[iDaughterBuilder], params);
  }
}

void NSVfitResonanceBuilderBase::print(std::ostream& stream) const 
{
  stream << "<NSVfitResonanceBuilderBase::print>:" << std::endl;
  stream << " pluginName = " << pluginName_ << std::endl;
  stream << " pluginType = " << pluginType_ << std::endl;
  stream << " prodResonanceLabel = " << prodResonanceLabel_ << std::endl;
  for ( std::vector<NSVfitSingleParticleBuilderBase*>::const_iterator daughterBuilder = daughterBuilders_.begin();
	daughterBuilder != daughterBuilders_.end(); ++daughterBuilder ) {
    (*daughterBuilder)->print(stream);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(NSVfitResonanceBuilderPluginFactory, "NSVfitResonanceBuilderPluginFactory");


