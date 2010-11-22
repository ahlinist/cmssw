#ifndef SusyAnalysis_AnalysisSkeleton_AddOtherVariables_h_
#define SusyAnalysis_AnalysisSkeleton_AddOtherVariables_h_
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
#include "DataFormats/Candidate/interface/Candidate.h"

namespace pat {
  class Jet;
}
class JetIDSelectionFunctor;
class PFJetIDSelectionFunctor;

class AddOtherVariables: public SusyEventSelector {
public:
   AddOtherVariables(const edm::ParameterSet&);
   virtual bool select(const edm::Event&) const;
   virtual ~AddOtherVariables() {
     //delete jetIDLooseCalo_; 
     //delete jetIDLoosePF_;
   }

protected:
   typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzV;
   double  dPhiBiased(const edm::View<reco::Candidate>*,  const edm::View<reco::Candidate>::const_iterator e)   const;
   std::vector<double> deltaSumPt_permutations(const std::vector<LorentzV>& p4s) const;
   double  alphaT(const std::vector<LorentzV>& p4s) const;   
   bool    isLoose(const pat::Jet*) const;
   
private:

   JetIDSelectionFunctor * jetIDLooseCalo_;
   PFJetIDSelectionFunctor * jetIDLoosePF_;

   edm::InputTag jetTag_;
   edm::InputTag metTag_;
   edm::InputTag electronTag_;///< tag for electron input collection
   edm::InputTag muonTag_;///< tag for muon     input collection
   edm::InputTag vertex_;///< tag for vertex
   //   edm::InputTag tauTag_;      ///< tag for tau      input collection
 
   double weight_, jetptmin_, jetetamax_;
   bool   useJetID_, rejectEvtJetID_;
};
#endif
