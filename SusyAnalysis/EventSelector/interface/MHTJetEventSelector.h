#ifndef SusyAnalysis_AnalysisSkeleton_MHTJetEventSelector_h_
#define SusyAnalysis_AnalysisSkeleton_MHTJetEventSelector_h_
/// Selection on delta phi MET / jets.
///
/// Performs a number of selections based on MET and jets topology:
/// - at least 3 jets (otherwise the variables are not calculated)
/// - minimum delta-phi between MET and any of the (up to N) leading jets
/// - minimum delta-phi between MET and second leading jet
/// - minimum R1 and R2 distances (between MET and 2 leading jets)
///
/// $Id $


// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
//#include "FWCore/ParameterSet/interface/InputTag.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"


class MHTJetEventSelector: public SusyEventSelector {
 public:
  MHTJetEventSelector(const edm::ParameterSet&);
  virtual bool select(const edm::Event&) const;
  virtual ~MHTJetEventSelector() {
  }

 protected:
  typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzV;

 private:
  edm::InputTag metTag_; ///< tag for input collection
  edm::InputTag jetTag_; ///< tag for input collection

  float mhtDphiMin_; ///< Minimum angle between MET and any leading jet
  float dPhiJet1MHTMin_; ///< Minimum angle between MET and 1nd leading jet
  float dPhiJet2MHTMin_; ///< Minimum angle between MET and 2nd leading jet
  float rDistJetsMin_; ///< Minimum R1 and R2
  unsigned int nJetsMHTIso_; ///<  Nr. of jets for MET isolation
  float minPt_; ///< minimum Pt of jets taken into account
  float maxEta_; ///< maximum Eta of jets taken into account
  bool useJetID_;
  bool rejectEvtJetID_;
};
#endif
