#include "SusyAnalysis/EventSelector/interface/MaxNumJetsEventSelector.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <DataFormats/Candidate/interface/Particle.h>

//________________________________________________________________________________________
MaxNumJetsEventSelector::MaxNumJetsEventSelector (const edm::ParameterSet& pset ) :
  SusyEventSelector(pset),
  jetTag_ (pset.getParameter<edm::InputTag>("jetTag") ), 
  maxEt_ ( pset.getParameter<double>("maxEt") ), // et cut on N+1th jet
  maxNumJets_ ( pset.getParameter<unsigned int>("maxNumJets") )// number of jets (N)
{
  // Define variables to store
  // All jet Ets
  for ( unsigned int i = 0 ; i != maxNumJets_+1 ; ++i ) {
    std::stringstream tempStream;
    tempStream << "jet" << i << "Et";
    defineVariable(tempStream.str().c_str());
  }
  // Total number of jets in event
  defineVariable("numJets");
}

//________________________________________________________________________________________
bool
MaxNumJetsEventSelector::select (const edm::Event& event) const
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

  // Store ET for jets up to N+1 if they exist
  for ( unsigned int i = 0 ; (i != jetHandle->size()) && (i != maxNumJets_+1) ; ++i ) {
    std::stringstream tempStream;
    tempStream << "jet" << i << "Et";
    if ( i >= jetHandle->size() ) setVariable(tempStream.str().c_str(),0.0);
    else setVariable(tempStream.str().c_str(),(*jetHandle)[i].et());
  }

  // Set the number of 'jets' in the event
  setVariable("numJets",static_cast<double>(jetHandle->size()));

  // TODO - check sorting of energies...

  // check number of jets
  // pass immediately if less than or equal to max
  if ( jetHandle->size() <= maxNumJets_ )  return true;

  // If not decide based on et cut of next most energetic jet
  if ( (*jetHandle)[maxNumJets_].et() > maxEt_ ) { 
    LogDebug("MaxNumJetsEventSelector") << "Jet "<< maxNumJets_ << " failed max Et cut";
    return false;
  }

  // Return selection
  return true;

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, MaxNumJetsEventSelector, "MaxNumJetsEventSelector");

