#include "SusyAnalysis/EventSelector/interface/AlphaSelector.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <DataFormats/Candidate/interface/Particle.h>

//________________________________________________________________________________________
AlphaSelector::AlphaSelector (const edm::ParameterSet& pset ) :
  SusyEventSelector(pset),
  jetTag_ (pset.getParameter<edm::InputTag>("jetTag") ), 
  minAlpha_ ( pset.getParameter<double>("minAlpha") )
{
  // Define variables to store
  defineVariable("twoJetMass");
  defineVariable("jet2Et");
  defineVariable("Alpha_Lisa");
}

//________________________________________________________________________________________
bool
AlphaSelector::select (const edm::Event& event) const
{

  // Reset cached variables
  resetVariables();

  // Check input collections
  edm::Handle< edm::View<pat::Jet> > jetHandle;
  event.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("JetEventSelector") << "No Jet results for InputTag " << jetTag_;
    return false;
  }
  if(jetHandle->size()<2)  {
    edm::LogWarning("JetEventSelector") << "To few jets to calculate Alpha_Lisa";
    return false;
  }

  // Compute variables
  float jet2Et = (*jetHandle)[1].et();
  float twoJetMass = ( (*jetHandle)[0].p4()+(*jetHandle)[1].p4() ).mass();
  float aAlpha = jet2Et/twoJetMass;

  // Store variables
  setVariable("jet2Et",jet2Et);
  setVariable("twoJetMass",twoJetMass);
  setVariable("Alpha_Lisa",aAlpha);

  // Return selection
  return (aAlpha > minAlpha_);

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, AlphaSelector, "AlphaSelector");

