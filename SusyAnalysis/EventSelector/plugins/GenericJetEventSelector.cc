#include "SusyAnalysis/EventSelector/interface/GenericJetEventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include <vector>
#include <sstream>

//________________________________________________________________________________________
GenericJetEventSelector::GenericJetEventSelector(const edm::ParameterSet& pset) :
   SusyEventSelector(pset), jetTag_(pset.getParameter<edm::InputTag> ("jetTag")), minPt_(pset.getParameter<std::vector<
            double> > ("minPt")), maxEta_(pset.getParameter<std::vector<double> > ("maxEta")), maxFem_(
            pset.getParameter<std::vector<double> > ("maxEMFraction")) {

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

   edm::LogInfo("GenericJetEventSelector") << "constructed with \n" << "  jetTag    = " << jetTag_ << "\n"
            << "  min #jets = " << minPt_.size();
}

//________________________________________________________________________________________
bool GenericJetEventSelector::select(const edm::Event& event) const {
   // reset cached variables
   resetVariables();

   //FIXME: what about checking at construction time?
   if (minPt_.size() != maxEta_.size() || maxFem_.size() != maxEta_.size()) {
      edm::LogError("GenericJetEventSelector") << "Inconsistent length of vector of cut values";
      return false;
   }

   // get the jets
   edm::Handle<edm::View<reco::Jet> > jetHandle;
   event.getByLabel(jetTag_, jetHandle);
   if (!jetHandle.isValid()) {
      edm::LogWarning("GenericJetEventSelector") << "No Jet results for InputTag " << jetTag_;
      return false;
   }
   //
   // check number of jets
   //
   setVariable(0, jetHandle->size());
   if (jetHandle->size() < minPt_.size())
      return false;
   //
   // check cuts (assume that jets are sorted by Pt)
   //
   bool result(false);

   unsigned int i = 0;
   for (edm::View<reco::Jet>::const_iterator jet = jetHandle->begin(); jet != jetHandle->end(); ++jet) {
      float EMFRAC = 0;
      const reco::CaloJet * cj = static_cast<const reco::CaloJet*> (&(*jet));
      if (!cj)
         EMFRAC = cj->emEnergyFraction();
      else {
         const reco::PFJet * pj = static_cast<const reco::PFJet*> (&(*jet));
         if (!pj)
            EMFRAC = pj->neutralEmEnergyFraction() + pj->chargedEmEnergyFraction();
      }

      if (jet->pt() >= minPt_[i] && fabs(jet->eta()) <= maxEta_[i] && EMFRAC <= maxFem_[i]) {
         setVariable(3* i + 1, jet->pt());
         setVariable(3* i + 2, jet->eta());
         setVariable(3* i + 3, EMFRAC);
         ++i;
      }
      if (i == minPt_.size()) {
         result = true;
         break;
      }
   }

   if (result)
      LogTrace("GenericJetEventSelector") << "GenericJetEventSelector: all jets passed";
   else
      LogTrace("GenericJetEventSelector") << "GenericJetEventSelector: failed";
   return result;
}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, GenericJetEventSelector, "GenericJetEventSelector");
