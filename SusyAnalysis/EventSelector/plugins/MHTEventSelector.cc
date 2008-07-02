#include "SusyAnalysis/EventSelector/interface/MHTEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

//__________________________________________________________________________________________________
MHTEventSelector::MHTEventSelector (const edm::ParameterSet& pset) :
  SusyEventSelector(pset),
  jetTag_( pset.getParameter<edm::InputTag>("jetTag") ),
  minMHT_ ( pset.getParameter<double>("minMHT") ),
  minPt_ ( pset.getParameter<double>("minPt") )
{ 

  // Store computed MHT
  defineVariable("MHT");

}

//__________________________________________________________________________________________________
bool
MHTEventSelector::select (const edm::Event& event) const
{
  // reset cached variables
  resetVariables();
  // Get the jets
  edm::Handle< edm::View<pat::Jet> > jetHandle;
  event.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("MHTEventSelector") << "No Jet results for InputTag " << jetTag_;
    return false;
  }

  // Sum over jet Ets (with cut on min. pt)
  float myMHT = 0.0;
  float sumpx = 0.0;
  float sumpy = 0.0;

  edm::View<pat::Jet>::const_iterator iJet = jetHandle->begin();
  while ( iJet != jetHandle->end() ) {
    if ( iJet->pt() > minPt_ ) {
      sumpx = sumpx - iJet->px();
      sumpy = sumpy - iJet->py();
    }
    ++iJet;
  }
  myMHT = sqrt(sumpx*sumpx+sumpy*sumpy);

  setVariable("MHT",myMHT);

  return myMHT > minMHT_;

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, MHTEventSelector, "MHTEventSelector");
