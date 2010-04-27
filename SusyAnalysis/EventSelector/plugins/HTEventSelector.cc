#include "SusyAnalysis/EventSelector/interface/HTEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

//__________________________________________________________________________________________________
HTEventSelector::HTEventSelector (const edm::ParameterSet& pset) :
  SusyEventSelector(pset),
  jetTag_( pset.getParameter<edm::InputTag>("jetTag") ),
  minHT_ ( pset.getParameter<double>("minHT") ),
  minPt_ ( pset.getParameter<double>("minPt") ),
  maxEta_ ( pset.getParameter<double>("maxEta") ),
  minFem_( pset.getParameter<double>("minEMFraction") ),
  maxFem_( pset.getParameter<double>("maxEMFraction") ),
  minN90_( pset.getParameter<int>("minTowersN90")),
  minfHPD_(pset.getParameter<double>("minfHPD"))
{ 

  // Store computed HT
  defineVariable("HT");

}

//__________________________________________________________________________________________________
bool
HTEventSelector::select (const edm::Event& event) const
{
  // reset cached variables
  resetVariables();
  // Get the jets
  edm::Handle< edm::View<pat::Jet> > jetHandle;
  event.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("HTEventSelector") << "No Jet results for InputTag " << jetTag_;
    return false;
  }

  // Sum over jet Ets (with cut on min. pt)
  float myHT = 0.0;
  edm::View<pat::Jet>::const_iterator iJet = jetHandle->begin();
  while ( iJet != jetHandle->end() ) {
    if(iJet->emEnergyFraction()<= minFem_  && fabs(iJet->eta())<2.6 ) continue;
    if(iJet->emEnergyFraction()>= maxFem_  && fabs(iJet->eta())<2.6 ) continue;
    if(iJet->jetID().n90Hits <= minN90_)  continue;
    if(iJet->jetID().fHPD >= minfHPD_ ) continue;
    if ( iJet->pt()>minPt_ && fabs(iJet->eta())<maxEta_ ) myHT += iJet->et();
    ++iJet;
  }

  setVariable("HT",myHT);

  return myHT>minHT_;

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, HTEventSelector, "HTEventSelector");
