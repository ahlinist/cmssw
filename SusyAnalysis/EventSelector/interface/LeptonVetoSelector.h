#ifndef SusyAnalysis_AnalysisSkeleton_LeptonVetoSelector_h_
#define SusyAnalysis_AnalysisSkeleton_LeptonVetoSelector_h_
/// Direct lepton veto
///
/// Selects events with no isolated electron/muon/tau below a certain Et threshold.
/// Isolation is computed relative to the lepton's Et.
/// The number of isolated leptons of each family is computed

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
//#include "FWCore/ParameterSet/interface/InputTag.h"

class LeptonVetoSelector: public SusyEventSelector {
public:
   LeptonVetoSelector(const edm::ParameterSet&);
   virtual bool select(const edm::Event&) const;
   virtual ~LeptonVetoSelector() {
   }
private:
   edm::InputTag electronTag_;///< tag for electron input collection
   edm::InputTag muonTag_;///< tag for muon     input collection
   edm::InputTag vertex_;///< tag for vertex
   double minPtEle_; ///< minimum pt for electrons to be checked
   double maxEtaEle_; ///< max abs(eta)
   double eleIso_; ///< limit on relative electron isolation (trackIso+ecalIso+hcalIso)/pt
   double eleIsoPF_; ///< limit on relative electron particle based isolation (trackIso+ecalIso+hcalIso)/pt
   double eleDxy_; ///< max dxy from beamSpot position
   double minPtMuon_; ///< minimum pt for muons
   double maxEtaMuon_; ///< max abs(eta
   double muonIso_; ///< limit on relative muon isolation (trackIso+ecalIso+hcalIso)/pt
   double muonIsoPF_; ///< limit on relative muon  particle based isolation (trackIso+ecalIso+hcalIso)/pt  for Particle Flow
   unsigned int muonHits_; ///< min number of hits
   double muonDxy_; ///< max dxy from beamSpot position
   bool invertVeto_  ;        ///< if lepton veto inverted


   //   edm::InputTag tauTag_;      ///< tag for tau      input collection
   //   float minEtTau_;            ///< minimum et for taus to be checked
   //   float tauIso_;              ///< limit on relative tau isolation (pt sum)

};
#endif
