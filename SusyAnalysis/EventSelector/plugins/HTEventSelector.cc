#include "SusyAnalysis/EventSelector/interface/HTEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

//__________________________________________________________________________________________________
HTEventSelector::HTEventSelector(const edm::ParameterSet& pset) :
   SusyEventSelector(pset),
   jetTag_(pset.getParameter<edm::InputTag> ("jetTag")),
   minHT_(pset.getParameter<double> ("minHT")),
   minPt_(pset.getParameter<double> ("minPt")),
   maxEta_(pset.getParameter<double> ("maxEta")),
   useJetID_(pset.getParameter<bool> ("useJetID")),
   rejectEvtJetID_(pset.getParameter<bool> ("rejectEvtJetID")) {

   // Store computed HT
   defineVariable("HT");

}

//__________________________________________________________________________________________________
bool HTEventSelector::select(const edm::Event& event) const {
   // reset cached variables
   resetVariables();
   // Get the jets
   edm::Handle<edm::View<pat::Jet> > jetHandle;
   event.getByLabel(jetTag_, jetHandle);
   if (!jetHandle.isValid()) {
      edm::LogWarning("HTEventSelector") << "No Jet results for InputTag " << jetTag_;
      return false;
   }

   //// To be set true if one jet is found failing jetID
   bool badJet = false;
   JetIDSelectionFunctor jetIDLoose( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE );
   pat::strbitset ret = jetIDLoose.getBitTemplate();

   // Sum over jet Ets (with cut on min. pt)
   float myHT = 0.0;
   edm::View<pat::Jet>::const_iterator iJet = jetHandle->begin();
   while (iJet != jetHandle->end()) {
      ret.set(false);
      bool loose = jetIDLoose(*iJet, ret);
      if (useJetID_ && !(loose)) {
         badJet = true;
         ++iJet;
         continue;
      }
      if (iJet->pt() > minPt_ && fabs(iJet->eta()) < maxEta_)
         myHT += iJet->et();
      ++iJet;
   }
   //std::cout << myHT << std::endl;
   setVariable("HT", myHT);

   return (myHT > minHT_ && (rejectEvtJetID_ && !(badJet)));

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, HTEventSelector, "HTEventSelector");
