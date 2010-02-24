#include "SusyAnalysis/EventSelector/interface/MyMHTEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"

//__________________________________________________________________________________________________
MyMHTEventSelector::MyMHTEventSelector (const edm::ParameterSet& pset) :
  SusyEventSelector(pset),
  jetTag_( pset.getParameter<edm::InputTag>("jetTag") ),
  minMHT_ ( pset.getParameter<double>("minMHT") ),
  minPt_ ( pset.getParameter<double>("minPt") ),
  maxEta_ ( pset.getParameter<double>("maxEta") )
{ 

  // Store computed HT
  defineVariable("myMHT");

}

//__________________________________________________________________________________________________
bool
MyMHTEventSelector::select (const edm::Event& event) const
{
  // reset cached variables

  math::XYZTLorentzVector HT;

  resetVariables();
  // Get the jets
  edm::Handle< edm::View<pat::Jet> > jetHandle;
  event.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("HTEventSelector") << "No Jet results for InputTag " << jetTag_;
    return false;
  }

  // Sum over jet Ets (with cut on min. pt)
  edm::View<pat::Jet>::const_iterator iJet = jetHandle->begin();
  while ( iJet != jetHandle->end() ) {
    if ( iJet->pt()>minPt_ && fabs(iJet->eta())<maxEta_ ) HT += iJet->correctedP4("abs");
    ++iJet;
  }
  float myMHT = HT.pt();

  setVariable("myMHT",myMHT);

  return myMHT>minMHT_;

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, MyMHTEventSelector, "MyMHTEventSelector");
