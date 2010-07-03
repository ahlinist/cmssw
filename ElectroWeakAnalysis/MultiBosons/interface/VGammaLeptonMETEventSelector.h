#ifndef ElectroWeakAnalysis_MultiBosons_VGammaLeptonMETEventSelector_h 
#define ElectroWeakAnalysis_MultiBosons_VGammaLeptonMETEventSelector_h 

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMuonSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaElectronSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMETSelector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

class VGammaLeptonMETEventSelector : public EventSelector {
 public:
  VGammaLeptonMETEventSelector( edm::ParameterSet const & params );
  
  virtual bool operator()( edm::EventBase const & t, pat::strbitset & ret);
  // for use as object selector
  bool operator()( const reco::CompositeCandidate&, edm::EventBase const & ) const;
  using EventSelector::operator();

  std::vector<reco::ShallowClonePtrCandidate>       const & selectedElectrons()        const { return selectedElectrons_;}
  std::vector<reco::ShallowClonePtrCandidate>       const & selectedMuons()            const { return selectedMuons_; }
  std::vector<reco::ShallowClonePtrCandidate>       const & selectedMETs()             const { return selectedMETs_; }
  std::vector<reco::ShallowClonePtrCandidate>       const & selectedMuonPlusMETs()     const { return selectedMuonPlusMETs_; }
  std::vector<reco::ShallowClonePtrCandidate>       const & selectedElectronPlusMETs() const { return selectedElectronPlusMETs_; }

  void printSelectors(std::ostream & out) {
    this->print(out);
    out << "Muon ID Selector: " << std::endl;
    muonId_.print(out);
    out << "Electron ID Selector: " << std::endl;
    electronId_.print(out);
    out << "MET Selector: " << std::endl;
    metId_.print(out);
  }
 
 protected: 

  double acop(const reco::CompositeCandidate& ) const;
  double mt(const reco::CompositeCandidate& ) const;

  edm::InputTag               trigTag_;
  edm::InputTag               muonTag_;
  edm::InputTag               electronTag_;
  edm::InputTag               metTag_;
  edm::InputTag               muonPlusMETTag_;
  edm::InputTag               electronPlusMETTag_;

  std::string                 muTrig_;
  std::string                 eleTrig_;

  std::vector<reco::ShallowClonePtrCandidate> selectedMuons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedElectrons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedMETs_;
  std::vector<reco::ShallowClonePtrCandidate> selectedMuonPlusMETs_;
  std::vector<reco::ShallowClonePtrCandidate> selectedElectronPlusMETs_;

  VGammaMuonSelector                   muonId_;
  VGammaElectronSelector               electronId_;
  VGammaMETSelector                    metId_;  

  double minMT;
  double minAcoplanarity;
};


#endif
