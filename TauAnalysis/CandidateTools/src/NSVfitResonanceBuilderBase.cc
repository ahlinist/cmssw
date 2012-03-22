#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceBuilderBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

NSVfitResonanceBuilderBase::NSVfitResonanceBuilderBase(const edm::ParameterSet& cfg)
  : NSVfitBuilderBase(cfg),
    prodResonanceLabel_(cfg.getParameter<std::string>("prodResonanceLabel")),
    numDaughterBuilders_(0)
{
// nothing to be done yet... 
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
  NSVfitResonanceHypothesis* resonance = new NSVfitResonanceHypothesis();

  reco::Candidate::LorentzVector p4(0,0,0,0);

  for ( std::vector<NSVfitSingleParticleBuilderBase*>::const_iterator daughterBuilder = daughterBuilders_.begin();
	daughterBuilder != daughterBuilders_.end(); ++daughterBuilder ) {
    NSVfitSingleParticleHypothesis* daughter = (*daughterBuilder)->build(inputParticles);
    daughter->setMother(resonance);

    p4 += daughter->p4();

    resonance->daughters_.push_back(daughter);
  }

  resonance->p4_ = p4;

  if ( resonance->numDaughters() == 2 ) {
    const NSVfitSingleParticleHypothesis* daughter1 = resonance->daughter(0);
    const NSVfitSingleParticleHypothesis* daughter2 = resonance->daughter(1);
    resonance->dPhiVis_ = TMath::ACos(TMath::Cos(daughter1->p4().phi() - daughter2->p4().phi()));
  }

  resonance->name_ = prodResonanceLabel_;

  resonance->barcode_ = barcodeCounter_;
  ++barcodeCounter_;

  return resonance;
}

void NSVfitResonanceBuilderBase::applyFitParameter(NSVfitResonanceHypothesis* resonance, const double* params) const
{
  for ( unsigned iDaughterBuilder = 0; iDaughterBuilder < numDaughterBuilders_; ++iDaughterBuilder ) {
    daughterBuilders_[iDaughterBuilder]->applyFitParameter(resonance->daughter(iDaughterBuilder), params);
  }

  reco::Candidate::LorentzVector dp4(0,0,0,0);

  size_t numDaughters = resonance->numDaughters();
  for ( size_t iDaughter = 0; iDaughter < numDaughters; ++iDaughter ) {
    const NSVfitSingleParticleHypothesis* daughter = resonance->daughter(iDaughter);
    dp4 += daughter->dp4_fitted();
  }
  
  resonance->dp4_ = dp4;

  if ( numDaughters == 2 ) {
    const NSVfitSingleParticleHypothesis* daughter1 = resonance->daughter(0);
    resonance->prod_angle_rf_ = SVfit_namespace::decayAngleFromLabMomenta(resonance->p4_fitted(), daughter1->p4_fitted());
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


