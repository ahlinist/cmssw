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
//#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMuonSelector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

class VGammaDiLeptonEventSelector : public EventSelector {
 public:
  VGammaDiLeptonEventSelector( edm::ParameterSet const & params );
  
  virtual bool operator()( edm::EventBase const & t, pat::strbitset & ret);
  using EventSelector::operator();

  std::vector<reco::ShallowClonePtrCandidate> const & selectedElectrons()   const { return selectedElectrons_;}
  std::vector<reco::ShallowClonePtrCandidate> const & selectedMuons()       const { return selectedMuons_; }
  std::vector<reco::ShallowClonePtrCandidate>       const & selectedDiMuons()     const { return selectedDiMuons_; }
  std::vector<reco::ShallowClonePtrCandidate>       const & selectedDiElectrons() const { return selectedDiElectrons_; }

  void printSelectors(std::ostream & out) {
    out << "Muon ID Selector: " << std::endl;
    muonId_.print(out);
    out << "DiMuon Muon ID Selector: " << std::endl;
    diMuonId_.print(out);
    //out << "Electron ID Selector: " << std::endl;
    //electronId_.print(out);
  }
 
 protected: 

  edm::InputTag               trigTag_;
  edm::InputTag               muonTag_;
  edm::InputTag               electronTag_;
  edm::InputTag               diMuonTag_;
  edm::InputTag               diElectronTag_;

  std::string                 muTrig_;
  std::string                 eleTrig_;

  std::vector<reco::ShallowClonePtrCandidate> selectedMuons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedElectrons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedDiMuons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedDiElectrons_;

  VGammaMuonSelector                   muonId_, diMuonId_;
  //  VGammaElectronSelector               electronId_, diElectronId_;

  bool doee, domumu;
  
};


#endif
