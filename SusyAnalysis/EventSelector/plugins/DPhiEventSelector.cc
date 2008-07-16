#include "SusyAnalysis/EventSelector/interface/DPhiEventSelector.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <DataFormats/Candidate/interface/Particle.h>

//________________________________________________________________________________________
DPhiEventSelector::DPhiEventSelector (const edm::ParameterSet& pset ) :
  SusyEventSelector(pset),
  jetTag_ (pset.getParameter<edm::InputTag>("jetTag") ), 
  maxDPhi_ ( pset.getParameter<double>("maxDPhi") )
{
  // Define variables to store
  defineVariable("dphi");
  localPi = acos(-1.0); 
}

//________________________________________________________________________________________
bool
DPhiEventSelector::select (const edm::Event& event) const
{

  // Reset cached variables
  resetVariables();

  // Check input collections
  edm::Handle< edm::View<pat::Jet> > jetHandle;
  event.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("DPhiEventSelector") << "No Jet results for InputTag " << jetTag_;
    return false;
  }
  if(jetHandle->size()<2)  {
    edm::LogWarning("DPhiEventSelector") << "Too few jets to calculate dijet dphi";
    return false;
  }

  // TODO - check sorting?

  // Compute variables
  float dphi = (*jetHandle)[0].phi() - (*jetHandle)[1].phi();
  // Correct for negatives
  if ( dphi < 0.0 ) dphi = -dphi;
  // Correct for wrapping
  if ( dphi > localPi ) dphi = 2.0 * localPi - dphi;

  // Store variables
  setVariable("dphi",dphi);

  // Return selection
  return (dphi < maxDPhi_);

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, DPhiEventSelector, "DPhiEventSelector");

