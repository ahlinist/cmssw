#include "SusyAnalysis/EventSelector/interface/HemisphereSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Hemisphere.h"
//#include "PhysicsTools/Utilities/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"
#include <vector>

//________________________________________________________________________________________
HemisphereSelector::HemisphereSelector (const edm::ParameterSet& pset) :
  SusyEventSelector(pset),
  hemisphereTag_( pset.getParameter<edm::InputTag>("hemisphereTag") ),
  dPhiHemispheresMin_( pset.getParameter<double>("dPhiHemispheresMin") ),
  dPhiHemispheresMax_( pset.getParameter<double>("dPhiHemispheresMax")   )
{

  // Define all variables we want to cache (and eventually plot...)
  defineVariable("dPhiHemispheres");
 

}

//________________________________________________________________________________________
bool
HemisphereSelector::select (const edm::Event& event) const
{

  // Reset cached variables
  resetVariables();


  // Get the hemispheres
  edm::Handle< edm::View<pat::Hemisphere> > hemisphereHandle;
  event.getByLabel(hemisphereTag_, hemisphereHandle);
  if ( !hemisphereHandle.isValid() ) {
    edm::LogWarning("HemisphereSelector") << "No Hemisphere results for InputTag " << hemisphereTag_;
    return false;
  }
  const edm::View<pat::Hemisphere>& hemispheres = (*hemisphereHandle); // For simplicity...

  //
  // Compute variables
  //


  //delta Phi between Hemisphere Axis
  double dPhiHemispheres = fabs( reco::deltaPhi(hemispheres[0].phi(),hemispheres[1].phi()) ); 
  setVariable("dPhiHemispheres",dPhiHemispheres); 
 

  //
  // Perform final selection
  //
  if ( dPhiHemispheres      < dPhiHemispheresMin_     ) return false;
  if ( dPhiHemispheres      > dPhiHemispheresMax_     ) return false;
 
  return true;

}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, HemisphereSelector, "HemisphereSelector");
