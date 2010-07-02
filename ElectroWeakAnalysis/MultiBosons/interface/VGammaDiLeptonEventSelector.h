#ifndef ElectroWeakAnalysis_MultiBosons_VGammaDiLeptonEventSelector_h 
#define ElectroWeakAnalysis_MultiBosons_VGammaDiLeptonEventSelector_h 

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMuonSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaElectronSelector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

class VGammaDiLeptonEventSelector : public EventSelector {
 public:
  VGammaDiLeptonEventSelector( edm::ParameterSet const & params );
  
  virtual bool operator()( edm::EventBase const & t, pat::strbitset & ret);
  // for use as object selector
  bool operator()( const reco::CompositeCandidate&, edm::EventBase const & ) const;
  using EventSelector::operator();

  std::vector<reco::ShallowClonePtrCandidate>       const & selectedElectrons()   const { return selectedElectrons_;}
  std::vector<reco::ShallowClonePtrCandidate>       const & selectedMuons()       const { return selectedMuons_; }
  std::vector<reco::ShallowClonePtrCandidate>       const & selectedDiMuons()     const { return selectedDiMuons_; }
  std::vector<reco::ShallowClonePtrCandidate>       const & selectedDiElectrons() const { return selectedDiElectrons_; }

  void printSelectors(std::ostream & out) {
    out << "Muon ID Selector 1: " << std::endl;
    muonId1_.print(out);
    out << "Muon ID Selector 2: " << std::endl;
    muonId2_.print(out);
    out << "Electron ID Selector 1: " << std::endl;
    electronId1_.print(out);
    out << "Electron ID Selector 2: " << std::endl;
    electronId2_.print(out);
  }
 
 protected: 

  edm::InputTag               trigTag_;
  edm::InputTag               muonTag_;
  edm::InputTag               electronTag_;
  edm::InputTag               diMuonTag_;
  edm::InputTag               diElectronTag_;

  std::string                 muTrig_;
  std::string                 eleTrig_;

  std::vector<pat::Muon> allMuons_;
  std::vector<pat::Electron> allElectrons_;

  std::vector<reco::ShallowClonePtrCandidate> selectedMuons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedElectrons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedDiMuons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedDiElectrons_;

  VGammaMuonSelector                   muonId1_, muonId2_;
  VGammaElectronSelector               electronId1_, electronId2_;
  
};


#endif
