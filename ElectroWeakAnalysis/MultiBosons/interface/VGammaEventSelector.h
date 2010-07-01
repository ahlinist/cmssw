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
//#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaLeptonPlusMETSelector.h"
//#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaPhotonPlusMETSelector.h"
//#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaPhotonSelector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

class VGammaEventSelector : public EventSelector {
 public:
  VGammaEventSelector( edm::ParameterSet const & params );
  
  virtual bool operator()( edm::EventBase const & t, pat::strbitset & ret);
  using EventSelector::operator();

  // quick accessors to selected daughters
  std::vector<reco::ShallowClonePtrCandidate> const & selectedDiMuons()     const 
    { return diLeptonId_.selectedDiMuons(); }
  std::vector<reco::ShallowClonePtrCandidate> const & selectedDiElectrons() const 
    { return diLeptonId_.selectedDiElectrons(); }
  std::vector<reco::ShallowClonePtrCandidate> const & selectedPhotons()     const 
    { return selectedPhotons_; }
  /* // these will be uncommented as the selectors are written
  std::vector<reco::ShallowClonePtrCandidate> const & selectedMuonPlusMETs() const 
    { return leptonPlusMETId_.selectedMuonPlusMETs(); }
  std::vector<reco::ShallowClonePtrCandidate> const & selectedElectronPlusMETs() const 
    { return leptonPlusMETId_.selectedElectronPlusMETs(); }  
  */

  // accessors to full VGamma objects
  /* // photon+MET will be turned on when the selector is written 
  std::vector<reco::ShallowClonePtrCandidate> const & selectedInvisibleZGammaCandidates() const 
    { return photonPlusMETId_.selectedPhotonPlusMETs(); }
  */
  std::vector<reco::ShallowClonePtrCandidate> const & selectedZEEGammaCandidates()     const 
    { return selectedZEEGammaCandidates_; }
  std::vector<reco::ShallowClonePtrCandidate> const & selectedZMuMuGammaCandidates()   const 
    { return selectedZMuMuGammaCandidates_; }
  std::vector<reco::ShallowClonePtrCandidate> const & selectedWENuGammaCandidates()    const 
    { return selectedWENuGammaCandidates_; }
  std::vector<reco::ShallowClonePtrCandidate> const & selectedWMuNuGammaCandidates()   const 
    { return selectedWMuNuGammaCandidates_; }

  void printSelectors(std::ostream & out) {
    out << "DiLepton Selector: " << std::endl;
    diLeptonId_.printSelectors(out);
    /*
    out << "Lepton+MET Selector: " << std::endl;
    _.printSelectors(out);
    out << "DiLepton Selector: " << std::endl;
    muonId_.printSelectors(out);
    out << "DiLepton Selector: " << std::endl;
    muonId_.printSelectors(out);
    */
  }
 
 protected: 

  edm::InputTag               trigTag_;
  edm::InputTag               photonTag_;
  edm::InputTag               zeegTag_;
  edm::InputTag               zmumugTag_;
  edm::InputTag               wenugTag_;
  edm::InputTag               wmunugTag_;
  edm::InputTag               photonMETTag_;

  std::string                 muTrig_;
  std::string                 eleTrig_;
  std::string                 photonTrig_; // Photon and met triggers not implemented yet.
  std::string                 metTrig_;    // These flags do nothing at the moment.

  std::vector<reco::ShallowClonePtrCandidate> selectedPhotons_;
  std::vector<reco::ShallowClonePtrCandidate> selectedZEEGammaCandidates_;
  std::vector<reco::ShallowClonePtrCandidate> selectedZMuMuGammaCandidates_;
  std::vector<reco::ShallowClonePtrCandidate> selectedWENuGammaCandidates_;  
  std::vector<reco::ShallowClonePtrCandidate> selectedWMuNuGammaCandidates_;

  VGammaDiLeptonEventSelector                   diLeptonId_;
  //VGammaLeptonPlusMETSelector            leptonPlusMETId_;
  //VGammaPhotonPlusMETSelector            photonPlusMETId_;
  //VGammaPhotonSelector                   photonId_;
  
};


#endif
