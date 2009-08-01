#ifndef TauAnalysis_Core_MuonHistManager_h  
#define TauAnalysis_Core_MuonHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include <vector>
#include <string>

class MuonHistManager : public HistManagerBase 
{
 public:  
  explicit MuonHistManager(const edm::ParameterSet&);
  ~MuonHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

//--- auxiliary functions
  void bookMuonHistograms(DQMStore&, MonitorElement*&, MonitorElement*&, MonitorElement*&, const char*);
  
  void fillMuonHistograms(const pat::Muon&, MonitorElement*, MonitorElement*, MonitorElement*);
  void fillMuonIsoHistograms(const pat::Muon&);
  void fillMuonIsoConeSizeDepHistograms(const pat::Muon&);

//--- configuration parameters
  edm::InputTag muonSrc_;
  edm::InputTag vertexSrc_;
  edm::InputTag genParticleSrc_;

  std::string dqmDirectory_store_;

  bool requireGenMuonMatch_;

  unsigned numMuonIsoConeSizes_;
  double muonIsoConeSizeIncr_;
  unsigned numMuonIsoPtThresholds_;
  double muonIsoPtThresholdIncr_;

//--- histograms
  MonitorElement* hNumMuons_;

  MonitorElement* hMuonPt_;
  MonitorElement* hMuonEta_;
  MonitorElement* hMuonPtVsEta_;
  MonitorElement* hMuonPhi_;

  MonitorElement* hMuonPtCompToGen_;
  MonitorElement* hMuonThetaCompToGen_;
  MonitorElement* hMuonPhiCompToGen_;

  MonitorElement* hMuonMatchingGenParticlePdgId_;

  MonitorElement* hMuonTrackIPxy_;
  MonitorElement* hMuonTrackIPz_;

  MonitorElement* hMuonEcalDeposits_;
  MonitorElement* hMuonHcalDeposits_;
  MonitorElement* hMuonCaloDeposits_;
  MonitorElement* hMuonCaloCompatibility_;

  MonitorElement* hMuonNumberOfChambers_;
  MonitorElement* hMuonSegmentCompatibility_;

//--- IsoDeposits reconstructed from ECAL and HCAL recHits/CaloTowers and reco::Tracks
  MonitorElement* hMuonTrkIsoPt_;
  MonitorElement* hMuonEcalIsoPt_;
  MonitorElement* hMuonHcalIsoPt_;
  MonitorElement* hMuonIsoSumPt_;
  MonitorElement* hMuonTrkIsoPtRel_;
  MonitorElement* hMuonEcalIsoPtRel_;
  MonitorElement* hMuonHcalIsoPtRel_;
  MonitorElement* hMuonIsoSumPtRel_;

  MonitorElement* hMuonTrkIsoValProfile_;
  MonitorElement* hMuonTrkIsoEtaDistProfile_;
  MonitorElement* hMuonTrkIsoPhiDistProfile_;

  MonitorElement* hMuonEcalIsoValProfile_;
  MonitorElement* hMuonEcalIsoEtaDistProfile_;
  MonitorElement* hMuonEcalIsoPhiDistProfile_;

  MonitorElement* hMuonHcalIsoValProfile_;
  MonitorElement* hMuonHcalIsoEtaDistProfile_;
  MonitorElement* hMuonHcalIsoPhiDistProfile_;

  std::vector<MonitorElement*> hMuonTrkIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonEcalIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonHcalIsoPtConeSizeDep_;

  reco::isodeposit::AbsVetos muonTrkIsoParam_;
  reco::isodeposit::AbsVetos muonEcalIsoParam_;
  reco::isodeposit::AbsVetos muonHcalIsoParam_;

//--- IsoDeposits reconstructed from Partcile Flow
  MonitorElement* hMuonParticleFlowIsoPt_;
  MonitorElement* hMuonPFChargedHadronIsoPt_;
  MonitorElement* hMuonPFNeutralHadronIsoPt_;
  MonitorElement* hMuonPFGammaIsoPt_;

  std::vector<MonitorElement*> hMuonParticleFlowIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonPFChargedHadronIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonPFNeutralHadronIsoPtConeSizeDep_;
  std::vector<MonitorElement*> hMuonPFGammaIsoPtConeSizeDep_;

  reco::isodeposit::AbsVetos muonParticleFlowIsoParam_;

  int dqmError_;
};

#endif  


