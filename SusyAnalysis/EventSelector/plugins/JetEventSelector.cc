#include "SusyAnalysis/EventSelector/interface/JetEventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <vector>
#include <sstream>

//________________________________________________________________________________________
JetEventSelector::JetEventSelector(const edm::ParameterSet& pset) :
  SusyEventSelector(pset), 
  jetTag_(pset.getParameter<edm::InputTag> ("jetTag")),
  minPt_(pset.getParameter<std::vector<double> > ("minPt")),
  maxEta_(pset.getParameter<std::vector<double> > ("maxEta")),
  minFem_(pset.getParameter<std::vector<double> > ("minEMFraction")),
  maxFem_(pset.getParameter<std::vector<double> > ("maxEMFraction")),
  minN90_(pset.getParameter<int> ("minTowersN90")),
  maxfHPD_(pset.getParameter<double> ("maxfHPD")),
  useJetID_(pset.getParameter<bool> ("useJetID"))
 {

   /// definition of variables to be cached
   defineVariable("NumberOfJets");
   for (size_t i = 0; i < minPt_.size(); ++i) {
      std::ostringstream strPt;
      strPt << "Jet" << i << "Pt";
      defineVariable(strPt.str());
      std::ostringstream strEta;
      strEta << "Jet" << i << "Eta";
      defineVariable(strEta.str());
      std::ostringstream strFem;
      strFem << "Jet" << i << "EMfraction";
      defineVariable(strFem.str());
   }

   edm::LogInfo("JetEventSelector") << "constructed with \n" << "  jetTag    = " << jetTag_ << "\n" << "  min #jets = "
            << minPt_.size();
}

//________________________________________________________________________________________
bool JetEventSelector::select(const edm::Event& event) const {
   // reset cached variables
   resetVariables();

   //FIXME: what about checking at construction time?
   if (minPt_.size() != maxEta_.size() || maxFem_.size() != maxEta_.size()) {
      edm::LogError("JetEventSelector") << "Inconsistent length of vector of cut values";
      return false;
   }

   // get the jets
   edm::Handle<pat::JetCollection> jetHandle;
   event.getByLabel(jetTag_, jetHandle);
   if (!jetHandle.isValid()) {
      edm::LogWarning("JetEventSelector") << "No Jet results for InputTag " << jetTag_;
      return false;
   }
   //
   // check number of jets
   //
   setVariable(0, jetHandle->size());
   if (jetHandle->size() < minPt_.size())
      return false;
   //
   // check cuts (assume that jets are sorted by Et)
   //
   bool result(false);
   unsigned int numPassed = 0;

   for (unsigned int i = 0; i < minPt_.size(); ++i) {

      //not re-sorted at the moment!
      float EMFRAC = 0;
      if ((*jetHandle)[i].isCaloJet())
         EMFRAC = (*jetHandle)[i].emEnergyFraction();
      if ((*jetHandle)[i].isPFJet())
         EMFRAC = (*jetHandle)[i].neutralEmEnergyFraction() + (*jetHandle)[i].chargedEmEnergyFraction();

      setVariable(3* numPassed + 1, (*jetHandle)[i].pt());
      setVariable(3* numPassed + 2, (*jetHandle)[i].eta());
      setVariable(3* numPassed + 3, EMFRAC);
      if ((*jetHandle)[i].pt() < minPt_[numPassed])
	continue;
      if (fabs((*jetHandle)[i].eta()) > maxEta_[numPassed])
	continue;
      if(EMFRAC > maxFem_[i] && fabs((*jetHandle)[i].eta()) < 2.6 && useJetID_)
	continue;
      if(EMFRAC < minFem_[i] && fabs((*jetHandle)[i].eta()) < 2.6 && useJetID_)
	continue;
      if ((*jetHandle)[i].jetID().n90Hits <= minN90_ && useJetID_)
         continue;
      if ((*jetHandle)[i].jetID().fHPD >= maxfHPD_ && useJetID_)
         continue;

      ++numPassed;

      if (numPassed == minPt_.size()) {
         result = true;
         break;
      }

   }


   if (result)
      LogTrace("JetEventSelector") << "JetEventSelector: all jets passed";
   else
      LogTrace("JetEventSelector") << "JetEventSelector: failed";
   return result;
}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, JetEventSelector, "JetEventSelector");

//// Example how to check if reco::Jet is pat::Jet
//   edm::Handle<edm::View<reco::Jet> > jetHandle;
//   event.getByLabel(jetTag_, jetHandle);
//   if (!jetHandle.isValid()) {
//      edm::LogWarning("JetEventSelector") << "No Jet results for InputTag " << jetTag_;
//      return false;
//   }
//   bool patjet = false;
//   try {
//      const pat::Jet testjet = dynamic_cast<const pat::Jet &> (*(jetHandle->begin()));
//      patjet = true;
//   } catch (...) {
//   }
//
//   for (size_t i = 0; i < jetHandle->size(); ++i) {
//      float pt = 0;
//      if (patjet) {
//         const pat::Jet *jet = dynamic_cast<const pat::Jet *> (&((*jetHandle)[i]));
//         pt = jet->pt();
//      } else {
//         const reco::Jet& jet = (*jetHandle)[i];
//         pt = jet.pt();
//      }
//   }
