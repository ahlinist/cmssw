#ifndef ElectroWeakAnalysis_MultiBosons_VGammaEventSelector_h 
#define ElectroWeakAnalysis_MultiBosons_VGammaEventSelector_h 

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaDiLeptonEventSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaLeptonMETEventSelector.h"
//#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaPhotonMETEventSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaPhotonSelector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

class VGammaEventSelector : public EventSelector {
 public:
  VGammaEventSelector( edm::ParameterSet const & params );
  
  virtual bool operator()( edm::EventBase const & t, pat::strbitset & ret);
  using EventSelector::operator();

  // all objects present in event of various types
  std::vector<pat::Photon>   const& allPhotons()   const { return allPhotons_; }
  std::vector<pat::Electron> const& allElectrons() const { return allElectrons_; }
  std::vector<pat::Muon>     const& allMuons()     const { return allMuons_; }
  std::vector<pat::MET>      const& allMETs()      const { return allMETs_; }

  std::vector<reco::CompositeCandidate> const& allDiMuons()          const { return allDiMuons_; }
  std::vector<reco::CompositeCandidate> const& allDiElectrons()      const { return allDiElectrons_; }
  std::vector<reco::CompositeCandidate> const& allMuonPlusMETs()     const { return allMuonPlusMETs_; }
  std::vector<reco::CompositeCandidate> const& allElectronPlusMETs() const { return allElectronPlusMETs_; }
  
  std::vector<reco::CompositeCandidate> const& allZMuMuGammaCands() const { return allZMuMuGammaCands_; }
  std::vector<reco::CompositeCandidate> const& allZEEGammaCands()  const { return allZEEGammaCands_; }
  std::vector<reco::CompositeCandidate> const& allWENuGammaCands()  const { return allWENuGammaCands_; }
  std::vector<reco::CompositeCandidate> const& allWMuNuGammaCands() const { return allWMuNuGammaCands_; }
  std::vector<reco::CompositeCandidate> const& allZNuNuGammaCands() const { return allZNuNuGammaCands_; }

  // photons  
  std::vector<reco::ShallowClonePtrCandidate> const& selectedZGammaPhotons()     const 
    { return selectedZGammaPhotons_; }
  std::vector<reco::ShallowClonePtrCandidate> const& selectedWGammaPhotons()     const 
    { return selectedWGammaPhotons_; }

  //muons
  std::vector<reco::ShallowClonePtrCandidate> const& selectedZGammaMuons()     const 
    { return selectedZGammaMuons_; }
  std::vector<reco::ShallowClonePtrCandidate> const& selectedWGammaMuons()     const 
    { return selectedWGammaMuons_; }

  //electrons
  std::vector<reco::ShallowClonePtrCandidate> const& selectedZGammaElectrons()     const 
    { return selectedZGammaElectrons_; }
  std::vector<reco::ShallowClonePtrCandidate> const& selectedWGammaElectrons()     const 
    { return selectedWGammaElectrons_; }

  //met
  std::vector<reco::ShallowClonePtrCandidate> const& selectedZGammaMETs()     const 
    { return selectedZGammaMETs_; }
  std::vector<reco::ShallowClonePtrCandidate> const& selectedWGammaMETs()     const 
    { return selectedWGammaMETs_; }
  
  //lepton+MET
  std::vector<reco::ShallowClonePtrCandidate> const& selectedWGammaMuonPlusMETs()     const 
    { return selectedWGammaMuonPlusMETs_; }
  std::vector<reco::ShallowClonePtrCandidate> const& selectedWGammaElectronPlusMETs()     const 
    { return selectedWGammaElectronPlusMETs_; }

  // dileptons
  std::vector<reco::ShallowClonePtrCandidate> const& selectedZGammaDiMuons()     const 
    { return selectedZGammaDiMuons_; }
  std::vector<reco::ShallowClonePtrCandidate> const& selectedZGammaDiElectrons()     const 
    { return selectedZGammaDiElectrons_; }


  // accessors to daughter selectors
  VGammaDiLeptonEventSelector      const& diLeptonSelector()      const { return diLeptonId_; }
  VGammaLeptonMETEventSelector const& leptonPlusMETSelector() const { return leptonPlusMETId_; }
  /* // will be uncommented as the selectors are written
  VGammaPhotonPlusMETEventSelector const& photonPlusMETSelector() const { return photonPlusMETId_; }
  */  

  // accessors to full VGamma objects
  std::vector<reco::ShallowClonePtrCandidate> const& selectedZEEGammaCands()     const 
    { return selectedZEEGammaCands_; }
  std::vector<reco::ShallowClonePtrCandidate> const& selectedZMuMuGammaCands()   const 
    { return selectedZMuMuGammaCands_; }
  std::vector<reco::ShallowClonePtrCandidate> const& selectedWENuGammaCands()    const 
    { return selectedWENuGammaCands_; }
  std::vector<reco::ShallowClonePtrCandidate> const& selectedWMuNuGammaCands()   const 
    { return selectedWMuNuGammaCands_; }
  /* // needs to be written
   std::vector<reco::ShallowClonePtrCandidate> const& selectedZNuNuGammaCands()   const 
    { return photonPlusMETId_.selectedPhotonPlusMETs(); }
  */

  void printSelectors(std::ostream & out) {
    out << "Event Selector: " << std::endl;
    this->print(out);
    if(verbose_) {
      out << "DiLepton Selector: " << std::endl;
      diLeptonId_.printSelectors(out);    
      out << "Lepton+MET Selector: " << std::endl;
      leptonPlusMETId_.printSelectors(out);
      out << "ZGamma Photon Selector: " << std::endl;
      zgphotonId_.print(out);
      out << "WGamma Photon Selector: " << std::endl;
      wgphotonId_.print(out);
      /*
	out << "DiLepton Selector: " << std::endl;
	muonId_.printSelectors(out);
	out << "DiLepton Selector: " << std::endl;
	muonId_.printSelectors(out);
      */
    }
  }
 
 protected: 

  void fillAll( edm::EventBase const & event );
  
  bool verbose_;

  edm::InputTag               trigTag_;
  edm::InputTag               muonTag_;
  edm::InputTag               electronTag_;  
  edm::InputTag               photonTag_;
  edm::InputTag               metTag_;
  edm::InputTag               dimuonTag_;
  edm::InputTag               dielectronTag_;
  edm::InputTag               electronMETTag_;
  edm::InputTag               muonMETTag_;
  edm::InputTag               zeegTag_;
  edm::InputTag               zmumugTag_;
  edm::InputTag               wenugTag_;
  edm::InputTag               wmunugTag_;
  edm::InputTag               znunugTag_;

  std::string                 muTrig_;
  std::string                 eleTrig_;
  std::string                 photonTrig_; // Photon and met triggers not implemented yet.
  std::string                 metTrig_;    // These flags do nothing at the moment.

  std::vector<pat::Photon>   allPhotons_;
  std::vector<pat::Muon>     allMuons_;
  std::vector<pat::Electron> allElectrons_;
  std::vector<pat::MET>      allMETs_;

  std::vector<reco::CompositeCandidate> allDiMuons_;
  std::vector<reco::CompositeCandidate> allDiElectrons_;
  std::vector<reco::CompositeCandidate> allMuonPlusMETs_;
  std::vector<reco::CompositeCandidate> allElectronPlusMETs_;

  std::vector<reco::CompositeCandidate> allZEEGammaCands_;
  std::vector<reco::CompositeCandidate> allZMuMuGammaCands_;
  std::vector<reco::CompositeCandidate> allWENuGammaCands_;
  std::vector<reco::CompositeCandidate> allWMuNuGammaCands_;
  std::vector<reco::CompositeCandidate> allZNuNuGammaCands_;
  
  std::vector<reco::ShallowClonePtrCandidate> selectedZGammaPhotons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedWGammaPhotons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedZGammaMuons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedWGammaMuons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedZGammaElectrons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedWGammaElectrons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedZGammaMETs_;
  std::vector<reco::ShallowClonePtrCandidate> selectedWGammaMETs_;
  std::vector<reco::ShallowClonePtrCandidate> selectedZGammaDiMuons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedZGammaDiElectrons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedWGammaMuonPlusMETs_;
  std::vector<reco::ShallowClonePtrCandidate> selectedWGammaElectronPlusMETs_;
  
  std::vector<reco::ShallowClonePtrCandidate> selectedZEEGammaCands_;
  std::vector<reco::ShallowClonePtrCandidate> selectedZMuMuGammaCands_;
  std::vector<reco::ShallowClonePtrCandidate> selectedWENuGammaCands_;  
  std::vector<reco::ShallowClonePtrCandidate> selectedWMuNuGammaCands_;

  VGammaDiLeptonEventSelector            diLeptonId_;
  VGammaLeptonMETEventSelector       leptonPlusMETId_;
  //VGammaPhotonPlusMETEventSelector            photonPlusMETId_;
  VGammaPhotonSelector                   zgphotonId_,wgphotonId_;
  
};


#endif
