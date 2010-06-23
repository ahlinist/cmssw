#include "SusyAnalysis/EventSelector/interface/MyMHTEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

//__________________________________________________________________________________________________
MyMHTEventSelector::MyMHTEventSelector(const edm::ParameterSet& pset) :
  SusyEventSelector(pset),
  jetTag_(pset.getParameter<edm::InputTag> ("jetTag")),
  minMHT_(pset.getParameter<double> ("minMHT")),
  maxMHT_(pset.getParameter<double> ("maxMHT")),
  minPt_(pset.getParameter<double> ("minPt")),
  maxEta_(pset.getParameter<double> ("maxEta")),
  minFem_(pset.getParameter<double> ("minEMFraction")),
  maxFem_(pset.getParameter<double> ("maxEMFraction")),
  minN90_(pset.getParameter<int> ("minTowersN90")),
  maxfHPD_(pset.getParameter<double> ("maxfHPD")),
  useJetID_(pset.getParameter<bool> ("useJetID"))  {

   // Store computed HT
   defineVariable("MHT");
}

//__________________________________________________________________________________________________
bool MyMHTEventSelector::select(const edm::Event& event) const {
   // reset cached variables
   math::XYZTLorentzVector HT;

   resetVariables();

   //   Get the jets
     edm::Handle<edm::View<pat::Jet> > jetHandle;
     event.getByLabel(jetTag_, jetHandle);
     if (!jetHandle.isValid()) {
        edm::LogWarning("MyMHTEventSelector") << "No Jet results for InputTag " << jetTag_;
        return false;
     }
//    edm::Handle<edm::View<reco::Jet> > jetHandle;
//    event.getByLabel(jetTag_, jetHandle);
//    if (!jetHandle.isValid()) {
//       edm::LogWarning("MyMHTEventSelector") << "No Jet results for InputTag " << jetTag_;
//       return false;
//    }

//    bool patjet = false;
//    bool calojet = false;
//    try {
//       const pat::Jet testjet = dynamic_cast<const pat::Jet &> (*(jetHandle->begin()));
//       patjet = true;
//       //std::cout << "Is pat::Jet" << std::endl;
//    } catch (...) {
//       try {
//          const reco::CaloJet testjet = dynamic_cast<const reco::CaloJet &> (*(jetHandle->begin()));
//          calojet = true;
//          //std::cout << "Is reco::CaloJet" << std::endl;
//       } catch (...) {
//       }
//    }

//   if (patjet) {

      // Sum over jet Ets (with cut on min. pt)
     //      edm::View<reco::Jet>::const_iterator iJet = jetHandle->begin();
      edm::View<pat::Jet>::const_iterator iJet = jetHandle->begin();
      while (iJet != jetHandle->end()) {
	//         const pat::Jet *jet = dynamic_cast<const pat::Jet *> (&(*iJet));
         //         std::cout << "pat::Jet (pt, eta, phi, emf, n90, fHPD): " << jet->pt() << ", " << jet->eta() << ", "
         //                  << jet->phi() << ", " << jet->emEnergyFraction() << ", " << jet->jetID().n90Hits << ", "
         //                  << jet->jetID().fHPD << std::endl;
         if (iJet->emEnergyFraction() <= minFem_ && fabs(iJet->eta()) < 2.6 && useJetID_) {
            ++iJet;
            continue;
         }
         if (iJet->emEnergyFraction() >= maxFem_ && fabs(iJet->eta()) < 2.6 && useJetID_) {
            ++iJet;
            continue;
         }
         if (iJet->jetID().n90Hits <= minN90_ && useJetID_) {
            ++iJet;
            continue;
         }
         if (iJet->jetID().fHPD >= maxfHPD_ && useJetID_) {
            ++iJet;
            continue;
         }
         if (iJet->pt() > minPt_ && fabs(iJet->eta()) < maxEta_) {
            math::XYZTLorentzVector p4(iJet->px(), iJet->py(), iJet->pz(), iJet->energy());//   iJet->correctedP4("abs");
            HT += p4;
         }
         ++iJet;
      }

//    } else if (calojet) {

//       // Sum over jet Ets (with cut on min. pt)
//       edm::View<reco::Jet>::const_iterator iJet = jetHandle->begin();
//       while (iJet != jetHandle->end()) {
//          const reco::CaloJet *jet = dynamic_cast<const reco::CaloJet *> (&(*iJet));
//          //         std::cout << "reco::CaloJet (pt, eta, phi, emf): " << jet->pt() << ", " << jet->eta() << ", " << jet->phi()
//          //                  << ", " << jet->emEnergyFraction() << std::endl;
//          if (jet->emEnergyFraction() <= minFem_ && fabs(jet->eta()) < 2.6) {
//             ++iJet;
//             continue;
//          }
//          if (jet->emEnergyFraction() >= maxFem_ && fabs(jet->eta()) < 2.6) {
//             ++iJet;
//             continue;
//          }
//          if (jet->pt() > minPt_ && fabs(jet->eta()) < maxEta_) {
//             math::XYZTLorentzVector p4(jet->px(), jet->py(), jet->pz(), jet->energy());//   jet->correctedP4("abs");
//             HT += p4;
//          }
//          ++iJet;
//       }

//    } else {

//       // Sum over jet Ets (with cut on min. pt)
//       edm::View<reco::Jet>::const_iterator iJet = jetHandle->begin();
//       while (iJet != jetHandle->end()) {
//          //         std::cout << "reco::Jet (pt, eta, phi): " << iJet->pt() << ", " << iJet->eta() << ", " << iJet->phi()
//          //                  << std::endl;
//          if (iJet->pt() > minPt_ && fabs(iJet->eta()) < maxEta_) {
//             math::XYZTLorentzVector p4(iJet->px(), iJet->py(), iJet->pz(), iJet->energy());//   iJet->correctedP4("abs");
//             HT += p4;
//          }
//          ++iJet;
//       }

//    }

   float myMHT = HT.pt();
   //std::cout << myMHT << std::endl;

   setVariable("MHT", myMHT);

   //
   // apply cut
   //
   bool result_max = true;
   if (maxMHT_ > 0)
      result_max = (myMHT < maxMHT_);
   bool result_min = true;
   if (minMHT_ > 0)
      result_min = (myMHT > minMHT_);
   return (result_min && result_max);

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, MyMHTEventSelector, "MyMHTEventSelector");
